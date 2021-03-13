#include <stdio.h>
#include <lcd_control.h>

int main(int argc, char const *argv[])
{
    lcd_init();

    printInScreen("Teste 1", "Teste 2");

    return 0;
}
