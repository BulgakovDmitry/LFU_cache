#ifndef SLOW_GET_PAGE
#define SLOW_GET_PAGE

inline int slow_get_page(std::size_t key) {
    return static_cast<int>(2*key);
}

#endif