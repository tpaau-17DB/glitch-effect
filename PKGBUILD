# Maintainer: Your Name <youremail@domain.com>
pkgname=glitch-effect
pkgver=1.0.1
pkgrel=2
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
md5sums=('e99b4660e44065cd677970110de51bd5')

build() {
    cd "$pkgname-$pkgver"
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
    ./copy-config
}
