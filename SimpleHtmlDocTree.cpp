//
// Created by viruser on 2022/12/27.
//

#include <string.h>
#include <optional>
#include "SimpleHtmlDocTree.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "errors.h"

using namespace rapidjson;
using std::static_pointer_cast;

BaseItem* parse_raw_item(const Value &raw_value);
std::string json_to_string(const Value &value);

SimpleHtmlDoc::~SimpleHtmlDoc() {}

SimpleHtmlDoc SimpleHtmlDoc::parse_raw(const std::string &raw_doc) {
    Document document;
    document.Parse(raw_doc.c_str(), raw_doc.length());
    return parse_raw(document);
}
SimpleHtmlDoc SimpleHtmlDoc::parse_raw(const char* raw_doc) {
    Document document;
    document.Parse(raw_doc);
    return parse_raw(document);
}

SimpleHtmlDoc SimpleHtmlDoc::parse_raw_from_cstr(const char *raw_doc) {
    return parse_raw(raw_doc);
}
SimpleHtmlDoc SimpleHtmlDoc::parse_raw(const Document &document) {
    if (!document.IsObject() || !document.HasMember("items") || !document["items"].IsArray()) {
        throw JsonParseError("document must contains items array!");
    }
    auto raw_items = document["items"].GetArray();
    std::vector<std::shared_ptr<BaseItem>> items;
    items.reserve(raw_items.Size()+10);
    for (const auto &item: raw_items) {
        try {
            items.push_back(std::shared_ptr<BaseItem>(parse_raw_item(item)));
        } catch (const JsonParseError &e) {
            throw JsonParseError(std::string ("item parse error, because:") + e.reason + json_to_string(item));
        }
    }
    return SimpleHtmlDoc{items};
}

std::string json_to_string(const Value &value) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    value.Accept(writer);
    return buffer.GetString();
}

inline int get_int(const Value &value, const char* key) {
//    value.FindMember(key)->value.IsInt()
    if (value.FindMember(key)->value.IsInt()) {
        return value[key].GetInt();
    }
    throw JsonParseError("field "+std::string(key)+" must be int.");
}
inline int get_int_or_default(const Value &value, const char* key, int default_value) {
    if (value.FindMember(key)->value.IsInt()) {
        return value[key].GetInt();
    }
    return default_value;
}

inline const char* get_text(const Value &value, const char* key="text") {
    if (value.FindMember(key)->value.IsString()) {
        return value[key].GetString();
    }
    throw JsonParseError("item text must be string!"+json_to_string(value));
}

inline const char* get_text_or_default(const Value &value, const char* key="text", const char* default_value="") {
    if (value.FindMember(key)->value.IsString()) {
        return value[key].GetString();
    }
    return default_value;
}

CellPos parse_pos(const Value &raw_value) {
    if (!raw_value.HasMember("pos") || !raw_value["pos"].IsArray()) {
        throw JsonParseError("pos must be int array");
    }
    auto value = raw_value["pos"].GetArray();
    if (value.Size() != 4) {
        throw JsonParseError("pos must be int array whitch len=4.");
    }
    for (const auto &item: value) {
        if (!item.IsInt()) {
            throw JsonParseError("pos must be int array whitch len=4.");
        }
    }
    return CellPos{
            value[0].GetInt(),
            value[1].GetInt(),
            value[2].GetInt(),
            value[3].GetInt(),
    };
}

BaseItem* parse_raw_item(const Value &raw_value) {
    if (!raw_value.IsObject() || !raw_value.HasMember("tag") || !raw_value["tag"].IsString()) {
        throw JsonParseError("item must be object, and has tag field!");
    }
    const char* tag = raw_value["tag"].GetString();

    int pid = get_int_or_default(raw_value, "pid", -1);
    int id = get_int(raw_value, "id");
    if (strcmp(tag, "p") == 0) {
        return new PItem(pid, id, get_text(raw_value));
    }
    else if (strcmp(tag, "span") == 0) {
        return new SpanItem(pid, id, get_text(raw_value));
    }
    else if (strcmp(tag, "cell") == 0) {
        return new CellItem(pid, id, parse_pos(raw_value));
    }
    else if (strcmp(tag, "div") == 0) {
        return new DivItem(pid, id);
    }
    else if (strcmp(tag, "table") == 0) {
        return new TableItem(pid, id);
    }
    else if (strcmp(tag, "img") == 0) {
        std::string format = get_text_or_default(raw_value, "fmt", "base64");
        return new ImgItem(pid, id, get_text(raw_value), format);
    }
    else {
        throw JsonParseError("unknown tag in item!");
    }
}

/**** SimpleHtmlDocTree ****/
SimpleHtmlDocTree::SimpleHtmlDocTree(std::vector<std::shared_ptr<BaseTreeItem>> items,
                                     std::map<int, std::shared_ptr<BaseTreeItem>> itemMap)
                                     : items(std::move(items)), itemMap(std::move(itemMap)) {}
std::optional<std::shared_ptr<BaseTreeItem>> SimpleHtmlDocTree::get_item(int item_id) {
    try {
        return itemMap.at(item_id);
    } catch (std::out_of_range &e) {
        return std::nullopt;
    }
}

std::optional<std::shared_ptr<BaseTreeItem>> SimpleHtmlDocTree::get_parent(const BaseTreeItem &item) {
    if (item.getPid() < 0) {
        return std::nullopt;
    }
    return this->get_item(item.getPid());
}

std::vector<std::shared_ptr<BaseTreeItem>> SimpleHtmlDocTree::find_path(int item_id) {
    std::vector<std::shared_ptr<BaseTreeItem>> result;
    find_path_inner(item_id, result);
    return result;
}
void SimpleHtmlDocTree::find_path_inner(int item_id, std::vector<std::shared_ptr<BaseTreeItem>> &result) {
    if (item_id < 0) {
        return;
    }
    auto item = get_item(item_id);
    if (!item.has_value()) {
        return;
    }
    find_path_inner(item.value()->getPid(), result);
    result.push_back(item.value());
}

//std::vector<std::shared_ptr<BaseTreeItem<BaseItem>>> SimpleHtmlDocTree::get_index(int item_id) {
//    return item
//}

SimpleHtmlDoc SimpleHtmlDocTree::to_shtm_doc() {
    std::vector<std::shared_ptr<BaseTreeItem>> flatTreeItems;
    iter_items(this->items, flatTreeItems);
    std::vector<std::shared_ptr<BaseItem>> baseItems;
    baseItems.reserve(flatTreeItems.size());
    for (const auto &item: flatTreeItems) {
        baseItems.push_back(item->item);
    }
    return SimpleHtmlDoc(baseItems);
}

static BaseTreeItem* wrapItem(const std::shared_ptr<BaseItem> &item) {
    if (item->tag == HtmlTag::span) {
        return new SpanTreeItem(static_pointer_cast<SpanItem>(item));
    } else if (item->tag == HtmlTag::p) {
        return new PTreeItem(static_pointer_cast<PItem>(item));
    } else if (item->tag == HtmlTag::img) {
        return new ImgTreeItem(static_pointer_cast<ImgItem>(item));
    } else if (item->tag == HtmlTag::div) {
        return new DivTreeItem(static_pointer_cast<DivItem>(item));
    } else if (item->tag == HtmlTag::cell) {
        return new CellTreeItem(static_pointer_cast<CellItem>(item));
    } else if (item->tag == HtmlTag::table) {
        return new TableTreeItem(static_pointer_cast<TableItem>(item));
    }
    throw IllegalStateError("unknown tag:"+ std::to_string(int(item->tag)));
}

SimpleHtmlDocTree SimpleHtmlDocTree::from_shtm_doc(const SimpleHtmlDoc &doc) {
    std::vector<std::shared_ptr<BaseTreeItem>> treeItems;
    std::map<int, std::shared_ptr<BaseTreeItem>> treeItemMap;
    for (const auto &item: doc.items) {
        auto itemPtr = std::shared_ptr<BaseTreeItem>(wrapItem(item));
        treeItems.push_back(itemPtr);
        treeItemMap[itemPtr->getId()] = itemPtr;
    }
    for (const auto &item: treeItems) {
        if (item->getPid() < 0) {
            continue;
        }
        auto parent = treeItemMap[item->getPid()];
        if (parent->getTag() == HtmlTag::div) {
            static_pointer_cast<DivTreeItem>(parent)->children.push_back(static_pointer_cast<SpanTreeItem>(item));
        }
        else if (parent->getTag() == HtmlTag::table) {
            static_pointer_cast<TableTreeItem>(parent)->children.push_back(static_pointer_cast<CellTreeItem>(item));
        }
        else if (parent->getTag() == HtmlTag::cell) {
            static_pointer_cast<CellTreeItem>(parent)->children.push_back(item);
        }
    }
    std::vector<std::shared_ptr<BaseTreeItem>> rootTreeItems;
    for (const auto &item: treeItems) {
        if (item->getPid() < 0) {
            rootTreeItems.push_back(item);
        }
    }
    return SimpleHtmlDocTree(rootTreeItems, treeItemMap);
}

template<typename T>
void SimpleHtmlDocTree::iter_items(std::vector<std::shared_ptr<T>> treeItems,
                std::vector<std::shared_ptr<BaseTreeItem>> &result) {
    for (const auto &item: treeItems) {
        result.push_back(item);
        if (item->getTag() == HtmlTag::div) {
            auto treeItem = (DivTreeItem*)&(*item);
            iter_items(treeItem->children, result);
        } else if (item->getTag() == HtmlTag::cell) {
            auto treeItem = (CellTreeItem*)&(*item);
            iter_items(treeItem->children, result);
        } else if (item->getTag() == HtmlTag::table) {
            auto treeItem = (TableTreeItem*)&(*item);
            iter_items(treeItem->children, result);
        }
    }
}
