//
// Created by viruser on 2022/12/27.
//

#include "shtmTreeItem.h"


static bool cellPosCom(const std::shared_ptr<CellTreeItem> &a, const std::shared_ptr<CellTreeItem> &b) {
    if (a->getPos().row == b->getPos().row) {
        return a->getPos().col < b->getPos().col;
    }
    return a->getPos().row < b->getPos().row;
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
    cellTexts.reserve(this->children.size());
    for (const auto &item: this->children) {
        cellTexts.push_back(item->getText());
    }
    return join(cellTexts.begin(), cellTexts.end(), "\n");
}

TableTreeItem::TableTreeItem(const std::shared_ptr<TableItem>& item):BaseTreeItem(item) {}
TableTreeItem::TableTreeItem(
        const std::shared_ptr<TableItem>& item,
        std::vector<std::shared_ptr<CellTreeItem>> children)
        :BaseTreeItem(item), children(std::move(children)) {}

std::string TableTreeItem::getText() const {
    auto tmpChildren(this->children);
    std::sort(tmpChildren.begin(), tmpChildren.end(),cellPosCom);
    std::vector<std::string> lines;
    std::vector<std::string> cellTexts;
    int lastRow = -1;
    for (const auto &item: tmpChildren) {
        if (item->getPos().row != lastRow) {
            if (!cellTexts.empty()) {
                lines.push_back(join(cellTexts.begin(), cellTexts.end(), "\t"));
            }
            cellTexts.clear();
            lastRow = item->getPos().row;
            continue;
        }
        cellTexts.push_back(item->getText());
    }
    if (!cellTexts.empty()) {
        lines.push_back(join(cellTexts.begin(), cellTexts.end(), "\t"));
    }
    return join(lines.begin(), lines.end(), "\n");
}
