{
  description = "dst";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems (system:
        f (import nixpkgs { inherit system; }));
    in
    {
      packages = forAllSystems (pkgs: {
        default = pkgs.stdenv.mkDerivation {
          pname = "dst";
          version = "dev";
          src = pkgs.lib.cleanSource ./.;
          nativeBuildInputs = [ pkgs.cmake ];
          buildInputs = [
            pkgs.libuv
            pkgs.cmocka
            pkgs.xorg.libX11
            pkgs.xorg.libxkbfile
          ];
          doCheck = true;
          meta = {
            description = "Dynamic system monitor for dwm";
            platforms = pkgs.lib.platforms.linux;
            mainProgram = "dst";
          };
        };
      });

      devShells = forAllSystems (pkgs: {
        default = pkgs.mkShell {
          nativeBuildInputs = [ pkgs.cmake ];
          buildInputs = [
            pkgs.libuv
            pkgs.cmocka
            pkgs.xorg.libX11
            pkgs.xorg.libxkbfile
          ];
        };
      });
    };
}
