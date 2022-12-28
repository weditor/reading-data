//
// Created by viruser on 2022/12/27.
//

#ifndef UNTITLED_SHTMITEM_H
#define UNTITLED_SHTMITEM_H

#include <string>
#include <utility>

enum class HtmlTag {
    p,
    div,
    span,
    table,
    cell,
    img,
};

struct CellPos {
    int row;
    int col;
    int rowspan;
    int colspan;
    CellPos(int row, int col, int rowspan, int colspan): row(row), col(col), rowspan(rowspan), colspan(colspan) {}
};

struct BaseItem {
    int pid;
    int id;
    HtmlTag tag;
    BaseItem(int pid, int id, HtmlTag tag): pid(pid), id(id), tag(tag) {}
};

struct SpanItem: public BaseItem {
    std::string text;

    SpanItem(int pid, int id, std::string text): BaseItem(pid, id, HtmlTag::span), text(std::move(text)) {}
};

struct PItem: public BaseItem {
    std::string text;

    PItem(int pid, int id, std::string text): BaseItem(pid, id, HtmlTag::p), text(std::move(text)) {}
};

struct ImgItem: public BaseItem {
    std::string text;
    std::string fmt; // base64 | url
    ImgItem(int pid, int id, std::string text, std::string fmt)
        : BaseItem(pid, id, HtmlTag::img), text(std::move(text)), fmt(std::move(fmt)) {}
};

struct DivItem: public BaseItem {
    DivItem(int pid, int id): BaseItem(pid, id, HtmlTag::div) {}
};
struct TableItem: public BaseItem {
    TableItem(int pid, int id): BaseItem(pid, id, HtmlTag::table) {}
};
struct CellItem: public BaseItem {
    const CellPos pos;
    CellItem(int pid, int id, const CellPos &pos): BaseItem(pid, id, HtmlTag::cell), pos(pos) {}
};


#endif //UNTITLED_SHTMITEM_H
