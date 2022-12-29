//
// Created by viruser on 2022/12/27.
//

#ifndef UNTITLED_SHTMTREEITEM_H
#define UNTITLED_SHTMTREEITEM_H

#include <vector>
#include <algorithm>
#include <memory>
#include "utils.h"
#include "shtmItem.h"


struct BaseTreeItem {
    const std::shared_ptr<BaseItem> item;

    explicit BaseTreeItem(const std::shared_ptr<BaseItem> &item): item(item) {}
    virtual std::string getText() const = 0;

    int getPid() const {return item->pid;}
    int getId() const {return item->id;}
    HtmlTag getTag() const {return item->tag;}
protected:
//    template<typename T>
//    T* getItem() {
//        return (T*)&(*(this->item));
//    };
};

struct SpanTreeItem: public BaseTreeItem {
    explicit SpanTreeItem(const std::shared_ptr<SpanItem> &item): BaseTreeItem(item){}

    std::string getText() const override {
        auto spanItem = (SpanItem*)&(*(this->item));
        return spanItem->text;
    }
};
struct PTreeItem : public BaseTreeItem {
    explicit PTreeItem(const std::shared_ptr<PItem>& item):BaseTreeItem(item){}
    std::string getText() const override {
        auto pItem = (PItem*)&(*(this->item));
        return pItem->text;
    }
};
struct ImgTreeItem: public BaseTreeItem {
    explicit ImgTreeItem(const std::shared_ptr<ImgItem>& item):BaseTreeItem(item){}

    std::string getText() const override {
        return "";
    }
};
struct DivTreeItem: public BaseTreeItem {
    explicit DivTreeItem(const std::shared_ptr<DivItem>& item):BaseTreeItem(item){}

    std::vector<std::shared_ptr<SpanTreeItem>> children;
    std::string getText() const override;
};
struct CellTreeItem: public BaseTreeItem {
    explicit CellTreeItem(const std::shared_ptr<CellItem>& item):BaseTreeItem(item){}

    std::vector<std::shared_ptr<BaseTreeItem>> children;
    std::string getText() const override;
    const CellPos& getPos() const {
        auto cellItem = (CellItem*)&(*(this->item));
        return cellItem->pos;
    }
};

struct TableTreeItem: public BaseTreeItem {
    TableTreeItem(const std::shared_ptr<TableItem>& item, std::vector<std::shared_ptr<CellTreeItem>> children);
    explicit TableTreeItem(const std::shared_ptr<TableItem>& item);

    std::vector<std::shared_ptr<CellTreeItem>> children;
    std::string getText() const override;
};

#endif //UNTITLED_SHTMTREEITEM_H
