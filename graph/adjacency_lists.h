#pragma once

#include <iostream>

#include "graph.h"
#include "vector"
#include "forward_list.h"

namespace Graph {

    template<typename T>
        class Adjacency_lists_base {
            public:
                using value_type = T;
                class Vertex;
            private:
                Vector<Vertex> vertices_;
                void update_vertices_this_link() {
                    for (auto& v : vertices_)
                        v.adjacency_lists_ = this;
                }
            public:
                Adjacency_lists_base() = default;
                Adjacency_lists_base(const Adjacency_lists_base& o) :vertices_(o.vertices_) {
                    update_vertices_this_link();
                }
                Adjacency_lists_base& operator=(const Adjacency_lists_base& o) {
                    auto copy = o;
                    std::swap(*this, copy);
                    return *this;
                }
                Adjacency_lists_base(Adjacency_lists_base&& o) :vertices_(std::move(o.vertices_)) {
                    update_vertices_this_link();
                }
                Adjacency_lists_base& operator=(Adjacency_lists_base&& o) {
                    std::swap(vertices_, o.vertices_);
                    update_vertices_this_link();
                    return *this;
                }

                Vertex& create_vertex(const T& t) {
                    vertices_.push_back(Vertex(t, vertices_.size(), this));
                    return vertices_[vertices_.size() - 1];
                }
                size_t vertices_count() const {
                    return vertices_.size();
                }
                const Vertex& vertex_at(size_t index) const {
                    return vertices_[index];
                }
                Vertex& vertex_at(size_t index) {
                    return vertices_[index];
                }
                bool has_edge(const Vertex& v, const Vertex& w) const {
                    return v.has_edge(w);
                }

                auto cbegin() const { return vertices_.cbegin(); }
                auto cend() const { return vertices_.cend(); }
                auto begin() { return vertices_.begin(); }
                auto end() { return vertices_.end(); } 

                void print_internal(std::ostream& stream) {
                    for (auto& v : vertices_) {
                        stream << v.index_ << ": ";
                        for (auto& w : v)
                            stream << w.index() << " ";
                        stream << std::endl;
                    }
                }
        };

    template<typename T, Graph_type graph_type = Graph_type::GRAPH>
        class Adjacency_lists : public Adjacency_lists_base<T> {
            public:
                using Vertex = typename Adjacency_lists_base<T>::Vertex;
                template<typename TT, Graph_type gt = Graph_type::GRAPH>
                    struct Edges_handler {
                        static void add_edge(Vertex& v1, Vertex& v2) {
                            v1.add_link(v2);
                            v2.add_link(v1);
                        }
                        static void remove_edge(Vertex& v1, Vertex& v2) {
                            v1.remove_edge(v2);
                            v2.remove_edge(v1);
                        }
                    };
                template<typename TT>
                    struct Edges_handler<TT, Graph_type::DIGRAPH> {
                        static void add_edge(Vertex& v1, Vertex& v2) {
                            v1.add_link(v2);
                        }
                        static void remove_edge(Vertex& v1, Vertex& v2) {
                            v1.remove_edge(v2);
                        }
                    };
            private:
                using Base = Adjacency_lists_base<T>;
            public:
                Adjacency_lists& add_edge(Vertex& v1, Vertex& v2) {
                    Edges_handler<T, graph_type>::add_edge(v1, v2);
                    return *this;
                }
                void remove_edge(Vertex& v1, Vertex& v2) {
                    Edges_handler<T, graph_type>::remove_edge(v1, v2);
                }
        };

    template<typename T>
        class Adjacency_lists_base<T>::Vertex : public Vertex_base<T> {
            private:
                template<bool T_is_const>
                    class Iterator;

                friend class Adjacency_lists_base;
                template<typename TT, Graph_type graph_type>
                    friend class Adjacency_lists;
                friend class Vector<Vertex>;

                Adjacency_lists_base* adjacency_lists_;
                Forward_list<size_t> links_;

                Vertex(T value, size_t index, Adjacency_lists_base* adjacency_lists)
                    :Vertex_base<T>(value, index), adjacency_lists_(adjacency_lists)
                {}
                Vertex() :adjacency_lists_(nullptr) {}

                void add_link(const Vertex& v) {
                    bool found = false;
                    for (auto link = links_.begin(); link != links_.end() && !found; ++link)
                        found = *link == v.index_;
                    if (!found)
                        links_.push_back(v.index_);
                }
            public:
                using iterator = Iterator<false>;
                using const_iterator = Iterator<true>;

                iterator begin() {
                    return {*this, links_.begin()};
                }
                iterator end() {
                    return {*this, links_.end()};
                }
                const_iterator cbegin() const {
                    return {*this, links_.cbegin()};
                }
                const_iterator cend() const {
                    return {*this, links_.cend()};
                }
                void remove_edge(const Vertex& v) {
                    links_.remove_first_if([&v](size_t index) { return v.index() == index; });
                }
                bool has_edge(const Vertex& v) const {
                    for (auto l = v.cbegin(); l != v.cend(); ++l)
                        if (*l == v)
                            return true;
                    return false;
                }
        };

    template<typename T>
        template<bool T_is_const>
        class Adjacency_lists_base<T>::Vertex::Iterator {
            private:
                using links_type = Forward_list<size_t>;
                using links_iterator_type = std::conditional_t<T_is_const,
                      links_type::const_iterator, links_type::iterator>;
                using value_type = std::conditional_t<T_is_const, const Vertex, Vertex>;

                friend class Vertex;
                value_type& vertex_;
                links_iterator_type it_;
                Iterator(value_type& vertex, const links_iterator_type& it)
                    :vertex_(vertex), it_(it)
                {}
            public:
                const Iterator& operator++() {
                    ++it_;
                    return *this;
                }
                bool operator==(const Iterator& o) const {
                    return it_ == o.it_;
                }
                bool operator!=(const Iterator& o) const {
                    return !operator==(o);
                }
                value_type* operator->() const {
                    return &operator*();
                }
                value_type& operator*() const {
                    return vertex_.adjacency_lists_->vertices_[*it_];
                }
        };
}
