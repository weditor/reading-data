//
// Created by viruser on 2022/12/27.
//

#include "shtmTreeItem.h"


static bool cellPosCom(CellTreeItem* a, CellTreeItem* b) {
    if (a->item.pos.row == b->item.pos.row) {
        return a->item.pos.col < b->item.pos.col;
    }
    return a->item.pos.row < b->item.pos.row;
}

std::string DivTreeItem::getText() const {
    std::string result;
    for (const auto &item: children) {
        result.append(item->getText());
    }
    return result;
}
std::string CellTreeItem::getText() const {
    std::vector<std::string> cellTexts;
    for (const auto &item: this->children) {
        cellTexts.push_back(item->getText());
    }
    return join(cellTexts.begin(), cellTexts.end(), "\n");
}

std::string TableTreeItem::getText() const {
    std::sort(this->children.begin(),this->children.end(),cellPosCom);
    std::vector<std::string> lines;
    std::vector<std::string> cellTexts;
    int lastRow = -1;
    for (const auto &item: this->children) {
        if (item->item.pos.row != lastRow) {
            if (!cellTexts.empty()) {
                lines.push_back(join(cellTexts.begin(), cellTexts.end(), "\t"));
            }
            cellTexts.clear();
            lastRow = item->item.pos.row;
            continue;
        }
        cellTexts.push_back(item->getText());
    }
    if (!cellTexts.empty()) {
        lines.push_back(join(cellTexts.begin(), cellTexts.end(), "\t"));
    }
    return join(lines.begin(), lines.end(), "\n");
}
