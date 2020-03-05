#pragma once

#include "adjacency_lists.h"

namespace Graph {

    template<typename T>
        class Flow_link {
            private:
                size_t source_;
                size_t target_;
                T cap_;
                T flow_;
            public:
                size_t source() const { return source_; }
        };

    template<typename T>
        class Flow_edge {
            private:
                using link_type = Flow_link<T>;
                link_type* link_;
            public:
                using value_type = T;
                link_type* link() const { return link_; }
        };

    template<typename VT, typename ET>
        class Flow_vertex 
        : public Adjacency_lists_ns::Adj_lists_vertex_base<Graph_type::GRAPH, VT, Flow_edge<ET>, Flow_vertex<VT, ET>>
    {
            private:
                using Base = Adjacency_lists_ns::Adj_lists_vertex_base<Graph_type::GRAPH, VT, Flow_edge<ET>, Flow_vertex<VT, ET>>;
                using adj_lists_type = typename Base::adj_lists_type;
            public:
                Flow_vertex(VT value, adj_lists_type* adjacency_lists)
                    :Base(value, adjacency_lists)
                {}
                Flow_vertex() :Base() {}
                ~Flow_vertex() {
                    for (auto e = Base::cedges_begin(); e != Base::cedges_end(); ++e) {
                        auto link = e->edge().link();
                        if (link->source() == Base::index())
                            delete link;
                    }
                }
        };

    template<typename V, typename C>
        class Flow : public Adjacency_lists_ns::Adjacency_lists<Graph_type::GRAPH, Flow_vertex<V, C>> {
            private:
                using Base = Adjacency_lists_ns::Adjacency_lists<Graph_type::GRAPH, Flow_vertex<V, C>>;
            public:
                using vertex_type = typename Base::vertex_type;
            void add_edge(const vertex_type& v, const vertex_type& w, C cap, C flow) {
                std::cout << "edge added" << std::endl;
            }
        };

}
