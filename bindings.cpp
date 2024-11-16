#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "round_manager.cpp"

namespace py = pybind11;

PYBIND11_MODULE(round_manager, m) {
    py::class_<RoundManager>(m, "RoundManager")
        .def(py::init<>())
        .def("dealCard", &RoundManager::dealCard)
        .def("initializeRound", &RoundManager::initializeRound);
}