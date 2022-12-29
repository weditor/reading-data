//
// Created by viruser on 2022/12/28.
//
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "SimpleHtmlDocTree.h"
#include "shtmItem.h"

namespace py = pybind11;

PYBIND11_MODULE(example, m) {

    m.doc() = "pybind11 example plugin"; // optional module docstring

//    m.def("add", &add, "A function that adds two numbers");
    py::class_<BaseItem, std::shared_ptr<BaseItem>>(m, "BaseItem")
            .def_readonly("pid", &SpanItem::pid)
            .def_readonly("id", &SpanItem::id)
            .def_readonly("tag", &SpanItem::tag)
            ;
    py::class_<SpanItem, std::shared_ptr<SpanItem>>(m, "SpanItem")
            .def(py::init<int, int, std::string>())
            .def_readonly("pid", &SpanItem::pid)
            .def_readonly("id", &SpanItem::id)
            .def_readonly("tag", &SpanItem::tag)
            .def_readonly("text", &SpanItem::text)
            ;
    py::class_<PItem, std::shared_ptr<PItem>>(m, "PItem")
            .def(py::init<int, int, std::string>())
            .def_readonly("pid", &PItem::pid)
            .def_readonly("id", &PItem::id)
            .def_readonly("tag", &PItem::tag)
            .def_readonly("text", &PItem::text)
            ;
    py::class_<ImgItem, std::shared_ptr<ImgItem>>(m, "ImgItem")
            .def(py::init<int, int, std::string, std::string>())
            .def_readonly("pid", &ImgItem::pid)
            .def_readonly("id", &ImgItem::id)
            .def_readonly("tag", &ImgItem::tag)
            .def_readonly("text", &ImgItem::text)
            .def_readonly("fmt", &ImgItem::fmt)
            ;
    py::class_<DivItem, std::shared_ptr<DivItem>>(m, "DivItem")
            .def(py::init<int, int>())
            .def_readonly("pid", &DivItem::pid)
            .def_readonly("id", &DivItem::id)
            .def_readonly("tag", &DivItem::tag)
            ;
    py::class_<TableItem, std::shared_ptr<TableItem>>(m, "TableItem")
            .def(py::init<int, int>())
            .def_readonly("pid", &TableItem::pid)
            .def_readonly("id", &TableItem::id)
            .def_readonly("tag", &TableItem::tag)
            ;
    py::class_<CellItem, std::shared_ptr<CellItem>>(m, "CellItem")
            .def(py::init<int, int, CellPos>())
            .def_readonly("pid", &CellItem::pid)
            .def_readonly("id", &CellItem::id)
            .def_readonly("tag", &CellItem::tag)
            .def_readonly("pos", &CellItem::pos)
            ;
    py::class_<CellPos>(m, "CellPos")
            .def(py::init<int, int, int, int>())
            .def_readonly("row", &CellPos::row)
            .def_readonly("col", &CellPos::col)
            .def_readonly("rowspan", &CellPos::rowspan)
            .def_readonly("colspan", &CellPos::colspan)
            ;
    /** TreeItem **/
    py::class_<SpanTreeItem>(m, "SpanTreeItem")
            .def(py::init<const std::shared_ptr<SpanItem> &>())
            .def("get_text", &SpanTreeItem::getText)
            ;
    py::class_<PTreeItem>(m, "PTreeItem")
            .def(py::init<const std::shared_ptr<PItem>&>())
            .def("get_text", &PTreeItem::getText)
            ;
    py::class_<ImgTreeItem>(m, "ImgTreeItem")
            .def(py::init<const std::shared_ptr<ImgItem>&>())
            .def("get_text", &ImgTreeItem::getText)
            ;
    py::class_<DivTreeItem>(m, "DivTreeItem")
            .def(py::init<const std::shared_ptr<DivItem>&>())
            .def_readonly("children", &DivTreeItem::children)
            .def("get_text", &DivTreeItem::getText)
            ;
    py::class_<CellTreeItem>(m, "CellTreeItem")
            .def(py::init<const std::shared_ptr<CellItem>&>())
            .def_readonly("children", &CellTreeItem::children)
            .def("get_text", &CellTreeItem::getText)
            ;
    py::class_<TableTreeItem>(m, "TableTreeItem")
            .def(py::init<const std::shared_ptr<TableItem>&>())
            .def_readonly("children", &TableTreeItem::children)
            .def("get_text", &TableTreeItem::getText)
            ;
    /** SimpleHtmlDocTree **/
    py::class_<SimpleHtmlDoc>(m, "SimpleHtmlDoc")
            .def_static("parse_raw", &SimpleHtmlDoc::parse_raw_from_cstr, "build SimpleHtmlDoc instance")
            .def_readonly("items", &SimpleHtmlDoc::items)
            ;
    py::enum_<HtmlTag>(m, "HtmlTag", py::arithmetic())
            .value("p", HtmlTag::p)
            .value("div", HtmlTag::div)
            .value("span", HtmlTag::span)
            .value("table", HtmlTag::table)
            .value("cell", HtmlTag::cell)
            .value("img", HtmlTag::img)
            ;
}
namespace PYBIND11_NAMESPACE {
    template<> struct polymorphic_type_hook<BaseItem> {
        static const void *get(const BaseItem *src, const std::type_info*& type) {
            // note that src may be nullptr
            if (!src) {
                return src;
            }
            if (src->tag == HtmlTag::p) {
                type = &typeid(PItem);
                return static_cast<const PItem*>(src);
            } else if (src->tag == HtmlTag::span) {
                type = &typeid(SpanItem);
                return static_cast<const SpanItem*>(src);
            } else if (src->tag == HtmlTag::div) {
                type = &typeid(DivItem);
                return static_cast<const DivItem*>(src);
            } else if (src->tag == HtmlTag::cell) {
                type = &typeid(CellItem);
                return static_cast<const CellItem*>(src);
            } else if (src->tag == HtmlTag::table) {
                type = &typeid(TableItem);
                return static_cast<const TableItem*>(src);
            } else if (src->tag == HtmlTag::img) {
                type = &typeid(ImgItem);
                return static_cast<const ImgItem*>(src);
            }
            return src;
        }
    };
} // namespace PYBIND11_NAMESPACE
