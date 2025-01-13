# Maintainer: Your Name <youremail@domain.com>
pkgname=glitch-effect
pkgver=1.0.0
pkgrel=1
epoch=0
pkgdesc="A command-line glitch effect running on ncurses."
arch=('x86_64')
url="https://github.com/tpaau-17DB/glitch-effect"
license=('GPL-3.0')
groups=()
depends=()
makedepends=('ncurses' 'base-devel' 'nlohmann-json')
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("https://github.com/tpaau-17DB/glitch-effect/archive/refs/tags/$pkgver.tar.gz")
noextract=()
md5sums=('f26c5ad72d97db2a4605716ee597bc0b')

build() {
    cd "$pkgname-$pkgver"
    autoreconf --install
	./configure
	make
}

check() {
    cd "$pkgname-$pkgver"
	make -k check
}

package() {
    cd "$pkgname-$pkgver"
    sudo make install
    ./copy-config.sh
}
