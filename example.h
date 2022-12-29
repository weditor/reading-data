//
// Created by viruser on 2022/12/29.
//

#ifndef UNTITLED_EXAMPLE_H
#define UNTITLED_EXAMPLE_H

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"
#include "SimpleHtmlDocTree.h"
#include "shtmItem.h"

typedef std::vector<std::shared_ptr<BaseItem>> BaseItemVector;
typedef std::vector<std::shared_ptr<BaseTreeItem>> BaseTreeItemVector;
PYBIND11_MAKE_OPAQUE(BaseItemVector);
PYBIND11_MAKE_OPAQUE(BaseTreeItemVector);

#endif //UNTITLED_EXAMPLE_H
