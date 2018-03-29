{ pkgs ? (import <nixpkgs> {})
, source ? ./.
, version ? "dev"
}:
with pkgs;
stdenv.mkDerivation rec {
  name = "dst-${version}";
  inherit version;
  src = lib.cleanSource source;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ libuv cmocka xorg.libX11 xorg.libxkbfile ];
  enableParallelBuilding = true;
  doCheck = true;
}