// CalculadoraIntereses.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <locale>
#include <sstream>
#include <iomanip>

// Función para configurar la consola para UTF-8
void ConfigurarConsola() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    system("chcp 65001");
#endif
}

// Función para calcular el monto de la cuota mensual
double CalcularMontoCuota(double capital, double tasaInteres, int anos) {
    int meses = anos * 12;
    double tasaMensual = tasaInteres / 12 / 100;
    return capital * tasaMensual / (1 - pow(1 + tasaMensual, -meses));
}

// Función para formatear números con separadores de miles
std::string FormatearNumero(double valor) {
    std::stringstream ss;
    ss.imbue(std::locale(""));  // Usando la configuración regional del sistema para el formato
    ss << std::fixed << std::setprecision(2) << valor;
    return ss.str();
}

// Función para calcular la tasa de interés
double CalcularTasaInteres(double capital, double cuota, int anos) {
    int meses = anos * 12;
    double tasaMin = 0, tasaMax = 100, tasaInteres;

    while ((tasaMax - tasaMin) > 0.0001) {
        tasaInteres = (tasaMin + tasaMax) / 2;
        double cuotaCalculada = capital * (tasaInteres / 12 / 100) / (1 - pow(1 + tasaInteres / 12 / 100, -meses));

        if (cuotaCalculada > cuota) {
            tasaMax = tasaInteres;
        }
        else {
            tasaMin = tasaInteres;
        }
    }
    return tasaInteres;
}

// Función GenerarTablaAmortizacion
void GenerarTablaAmortizacion(double capital, double tasaInteres, int anos, double cuota) {
    double saldo = capital, abonoCapital, intereses, tasaMensual = tasaInteres / 12 / 100;

    printf("\nMonto Total: %s\n\n", FormatearNumero(capital).c_str());
    printf("%10s %20s %20s %20s\n", "Cuota", "Abono Capital", "Intereses", "Restante");

    for (int i = 1; i <= anos * 12; ++i) {
        intereses = saldo * tasaMensual;
        abonoCapital = cuota - intereses;
        saldo -= abonoCapital;
        if (saldo < 0) saldo = 0;

        printf("%10s %20s %20s %20s\n",
            FormatearNumero(cuota).c_str(),
            FormatearNumero(abonoCapital).c_str(),
            FormatearNumero(intereses).c_str(),
            FormatearNumero(saldo).c_str());
    }
}

// Función para validar y obtener un número entero
int ObtenerEntero(const std::string& mensaje, int minimo, int maximo) {
    int valor;
    std::cout << mensaje;
    while (!(std::cin >> valor) || valor < minimo || valor > maximo) {
        std::cout << "Valor no válido. Intente de nuevo: ";
        std::cin.clear(); // Limpiar el estado de error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
    }
    return valor;
}

// Función para validar y obtener un número decimal
double ObtenerDecimal(const std::string& mensaje, double minimo, double maximo) {
    double valor;
    std::cout << mensaje;
    while (!(std::cin >> valor) || valor < minimo || valor > maximo || std::cin.peek() != '\n') {
        std::cout << "Valor no válido. Intente de nuevo: ";
        std::cin.clear(); // Limpiar el estado de error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
    }
    return valor;
}

// Función para mostrar el menú y pedir datos al usuario
void MostrarMenu() {
    int opcion, anos;
    double capital, tasaInteres, cuota;
    std::string cuotaFormateada;

    while (true) { // Bucle infinito para mostrar el menú
        std::cout << "1. Calcular Monto de Cuotas de un Préstamo\n";
        std::cout << "2. Calcular Tasa de Interés\n";
        std::cout << "3. Salir\n";
        std::cout << "Elige una opción: ";
        std::cin >> opcion;

        if (opcion == 3) break; // Salir del menú

        switch (opcion) {
        case 1:
            capital = ObtenerDecimal("\nIngrese el monto del préstamo (mayor a 1,000.00): ", 1000, std::numeric_limits<double>::max());
            tasaInteres = ObtenerDecimal("Ingrese el interés (% anual) (mayor a 1.00): ", 1, 50);
            anos = ObtenerEntero("Ingrese el tiempo (años) (mayor a 1): ", 1, 20);

            cuota = CalcularMontoCuota(capital, tasaInteres, anos);
            cuotaFormateada = FormatearNumero(cuota);
            printf("\nMonto de la cuota mensual: %s\n", cuotaFormateada.c_str());

            opcion = ObtenerEntero("\n¿Desea ver la tabla de amortización? (1 para sí, 2 para no): ", 1, 2);
            if (opcion == 1) {
                GenerarTablaAmortizacion(capital, tasaInteres, anos, cuota);

                std::cout << "\nPresione Enter para continuar...\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la línea
                std::cin.get(); // Espera a que el usuario presione Enter
            }
            break;
        case 2:
            capital = ObtenerDecimal("\nIngrese el monto del préstamo: ", 1000, std::numeric_limits<double>::max());
            cuota = ObtenerDecimal("Ingrese el monto de la cuota: ", 1, std::numeric_limits<double>::max());
            anos = ObtenerEntero("Ingrese el tiempo (años): ", 1, 20);

            tasaInteres = CalcularTasaInteres(capital, cuota, anos);
            printf("Tasa de interés: %.2f%%\n", tasaInteres); // Añade '%%' para imprimir el signo de porcentaje

            std::cout << "\nPresione Enter para continuar...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la línea
            std::cin.get(); // Espera a que el usuario presione Enter
            break;
        default:
            std::cout << "Opción no válida. Intente de nuevo." << std::endl;
        }

        // Limpiar el estado de error y descartar la entrada incorrecta
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
    }
}


int main() {
    ConfigurarConsola();
    MostrarMenu();
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
