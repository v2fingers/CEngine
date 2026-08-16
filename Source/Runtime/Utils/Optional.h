#define optional_type(type)                                                    \
  struct {                                                                     \
    bool present;                                                              \
    type value;                                                                \
  }
