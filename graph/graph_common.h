#pragma once

namespace Graph {

    enum class Graph_type {
        GRAPH, DIGRAPH, FLOW
    };

    template<typename T>
        class Vertex_base {
            protected:
                T value_;
                Vertex_base(const T& value) :value_(value) {}
                Vertex_base() = default;
            public:
                using value_type = T;
                const T& value() const { return value_; }
                void set_value(const T& value) { value_ = value; }
                bool operator==(const Vertex_base& o) const {
                    return this == &o;
                }
                bool operator!=(const Vertex_base& o) const {
                    return !operator==(o);
                }
                friend std::ostream& operator<<(std::ostream& stream, const Vertex_base& v) {
                    return stream << v.value_;
                }
        };

    template<typename V, typename E, bool T_is_const>
        class Edges_iterator_entry_base {
            protected:
                using vertex_type = std::conditional_t<T_is_const, const V, V>;
                using edge_type = std::conditional_t<T_is_const, const E, E>;
            public:
                vertex_type* source_;
                vertex_type* target_;
                Edges_iterator_entry_base() = default; 
                Edges_iterator_entry_base(vertex_type* source) :source_(source) {}
                vertex_type& source() const { return *source_; }
                vertex_type& target() const { return *target_; }
        };

    template<typename V, typename E, bool T_is_const, typename ET = typename E::value_type>
        class Edges_iterator_entry : public Edges_iterator_entry_base<V, E, T_is_const> {
            private:
                using Base = Edges_iterator_entry_base<V, E, T_is_const>;
            public:
                using vertex_type = typename Base::vertex_type;
                using edge_type = typename Base::edge_type;
                edge_type* edge_;
                Edges_iterator_entry() = default;
                Edges_iterator_entry(vertex_type* source) :Base(source) {}
                edge_type& edge() const { return *edge_; }
        };

    template<typename V, typename E, bool T_is_const>
        class Edges_iterator_entry<V, E, T_is_const, bool> : public Edges_iterator_entry_base<V, E, T_is_const> {
            private:
                using Base = Edges_iterator_entry_base<V, E, T_is_const>;
            public:
                using vertex_type = typename Base::vertex_type;
                using edge_type = typename Base::edge_type;
                Edges_iterator_entry() = default;
                Edges_iterator_entry(vertex_type* source) :Base(source) {}
        };

}