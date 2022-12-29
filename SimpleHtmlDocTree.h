//
// Created by viruser on 2022/12/27.
//

#ifndef UNTITLED_SIMPLEHTMLDOCTREE_H
#define UNTITLED_SIMPLEHTMLDOCTREE_H

#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include <map>
#include "shtmTreeItem.h"
#include "rapidjson/document.h"

class SimpleHtmlDocTree;

class SimpleHtmlDoc {
public:
    explicit SimpleHtmlDoc(std::vector<std::shared_ptr<BaseItem>> items): items(std::move(items)){};
    ~SimpleHtmlDoc();
public:
    const std::vector<std::shared_ptr<BaseItem>> items;

    static SimpleHtmlDoc parse_raw(const std::string &);
    static SimpleHtmlDoc parse_raw(const char *);
    static SimpleHtmlDoc parse_raw(const rapidjson::Document &);
    static SimpleHtmlDoc parse_raw_from_cstr(const char *);
};

class SimpleHtmlDocTree {
public:
    SimpleHtmlDocTree(std::vector<std::shared_ptr<BaseTreeItem>> items, std::map<int, std::shared_ptr<BaseTreeItem>> itemMap);
public:
    std::vector<std::shared_ptr<BaseTreeItem>> items;
    std::map<int, std::shared_ptr<BaseTreeItem>> itemMap;

public:
    std::optional<std::shared_ptr<BaseTreeItem>> get_item(int item_id);
    std::optional<std::shared_ptr<BaseTreeItem>> get_parent(const BaseTreeItem &item);
    std::vector<std::shared_ptr<BaseTreeItem>> find_path(int item_id);
//    std::vector<std::shared_ptr<BaseTreeItem>> get_index(int item_id);
    SimpleHtmlDoc to_shtm_doc();
    static SimpleHtmlDocTree from_shtm_doc(const SimpleHtmlDoc &doc);
private:
    void find_path_inner(int item_id, std::vector<std::shared_ptr<BaseTreeItem>> &result);
    template<typename T>
    void iter_items(std::vector<std::shared_ptr<T>> items,
                    std::vector<std::shared_ptr<BaseTreeItem>> &result);
};

/*************** 标注的文档 ****************/

//enum class AnnotStatus {
//    initial,
//    annotate,
//    checking,
//    checked,
//    black,
//};
//
//enum class CheckStatus {
//    initial = 0,
//    passed = 1,
//    failed = 2,
//};
//
//struct AnnotCheck {
//    std::string ant_id;
//    std::string user;
//    std::string time;
//    CheckStatus status;
//};
//
//struct AnnotBlock {
//    std::string ant_id;
//    std::string text;
//    std::string start;
//    std::string end;
//};
//struct AnnotQaPair {
//    std::string question_key;
//    std::vector<AnnotBlock> blocks;
//};
//struct AnnotGroup {
//    std::string ant_id;
//    std::string group_key;
//    std::vector<AnnotQaPair> qa_pairs;
//};
//struct AnnotShtmDoc {
//    std::string key;
//    AnnotStatus annot_status;
//    SimpleHtmlDoc content;
//    std::vector<AnnotGroup> annotations;
//    std::map<std::string, AnnotCheck> checks;
//};

#endif //UNTITLED_SIMPLEHTMLDOCTREE_H
