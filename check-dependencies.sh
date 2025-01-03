echo -n "g++: "
command -v g++ >/dev/null 2>&1 && echo "OK" || echo "NO"

echo -n "make: "
command -v make >/dev/null 2>&1 && echo "OK" || echo "NO"

echo -n "ncurses: "
pkg-config --exists ncurses && echo "OK" || echo "NO"

echo -n "nlohmann/json: "
find /usr/include -name 'json.hpp' > /dev/null 2>&1 && echo "OK" || echo "NO"
