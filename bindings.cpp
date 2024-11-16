#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "deck.h"
#include "round_manager.cpp"

namespace py = pybind11;

PYBIND11_MODULE(round_manager, m) {
    py::class_<RoundManager>(m, "RoundManager")
        .def(py::init<>())
        .def("dealCard", &RoundManager::dealCard)
        .def("initializeRound", &RoundManager::initializeRound);
    // py::class_<Deck>(m, "Deck")
    //     .def(py::init<int>())
    //     .def("viewDeck", &Deck::viewDeck)
    //     .def("dealCard", py::overload_cast<void>(&Deck::dealCard))
    //     .def("dealCard", py::overload_cast<int>(&Deck::dealCard));
}