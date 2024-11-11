echo -n "g++: "
command -v g++ >/dev/null 2>&1 && echo "OK" || echo "NO"

echo -n "ncurses: "
pkg-config --exists ncurses && echo "OK" || echo "NO"

echo -n "nlohmann/json: "
if [ -f "$(find /usr/include -name 'json.hpp')" ]; then
  echo "OK"
else
  echo "NO"
fi
