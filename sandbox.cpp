# include "src/ffly_client.hpp"
int main() {
    mdl::ffly_client fflyc(12, 12, 12);
    fflyc.initialize(128, 128, "FireFly Engine");
}
