// CalculadoraIntereses.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
    Prop�sito del programa: Realizar un programa C++ que calcule las cuotas a pagar de un prestamo y del interes.

    Grupo: Loida Prenza - ID: 1114929,
            Jos� Hernandez - ID: 1117038
            Merarys Nu�ez - ID: 1116733,
            Manuel Tejeda - ID: 1116661,
            Elahine Molina - ID: 1100373,
            Yeikol Lorenzo - ID: 1116873

    Fecha: 04/01/2024
 */

#include <iostream>
#include <cmath>
#include <locale>
#include <sstream>
#include <iomanip>

// Funci�n para configurar la consola para UTF-8
void ConfigurarConsola() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    system("chcp 65001");
#endif
}

// Funci�n para calcular el monto de la cuota mensual
double CalcularMontoCuota(double capital, double tasaInteres, int anos) {
    int meses = anos * 12;
    double tasaMensual = tasaInteres / 12 / 100;
    return capital * tasaMensual / (1 - pow(1 + tasaMensual, -meses));
}



// Funci�n para formatear n�meros con separadores de miles
std::string FormatearNumero(double valor) {
    std::stringstream ss;
    ss.imbue(std::locale(""));  // Usando la configuraci�n regional del sistema para el formato
    ss << std::fixed << std::setprecision(2) << valor;
    return ss.str();
}


// Funci�n para calcular la tasa de inter�s
double CalcularTasaInteres(double capital, double cuota, int anos) {
    int meses = anos * 12;
    double tasaMin = 0, tasaMax = 100, tasaInteres = -1; // Inicializar tasaInteres con -1

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

    // Verificar si la tasa de inter�s es v�lida
    if (tasaInteres == -1 || std::isnan(tasaInteres) || std::isinf(tasaInteres)) {
        // Si la tasa de inter�s no es v�lida, imprimir un mensaje y devolver un valor especial
        std::cout << "La proporci�n entre los valores proporcionados no permite obtener una tasa de inter�s v�lida." << std::endl;
        return -1; // Devolver -1 o cualquier otro valor que signifique "no v�lido"
    }

    return tasaInteres;
}


// Funci�n GenerarTablaAmortizacion
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

double calculoIntereses(double capital, double tasaInteres, int anos, double cuota) {
    double totalIntereses = 0;
    double saldo = capital, abonoCapital, intereses, tasaMensual = tasaInteres / 12 / 100;

    for (int i = 1; i <= anos * 12; ++i) {
        intereses = saldo * tasaMensual;
        abonoCapital = cuota - intereses;
        saldo -= abonoCapital;
        if (saldo < 0) saldo = 0;

        totalIntereses += intereses;
    }

    return totalIntereses;
}

// Funci�n para validar y obtener un n�mero entero
int ObtenerEntero(const std::string& mensaje, int minimo, int maximo) {
    int valor;
    std::cout << mensaje;
    while (!(std::cin >> valor) || valor < minimo || valor > maximo) {
        std::cout << "Valor no v�lido. Intente de nuevo: ";
        std::cin.clear(); // Limpiar el estado de error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
    }
    return valor;
}

// Funci�n para validar y obtener un n�mero decimal
double ObtenerDecimal(const std::string& mensaje, double minimo, double maximo) {
    double valor;
    std::cout << mensaje;
    while (!(std::cin >> valor) || valor < minimo || valor > maximo || std::cin.peek() != '\n') {
        std::cout << "Valor no v�lido. Intente de nuevo: ";
        std::cin.clear(); // Limpiar el estado de error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
    }
    return valor;
}

// Funci�n para mostrar el men� y pedir datos al usuario
void MostrarMenu() {
    bool inicial = false;
    int opcion, anos, inicialOpcion, porcientoInicial;
    double capital, tasaInteres, cuota, totalIntereses, calculoInicial, restanteCapital;
    std::string input;
    std::string cuotaFormateada;
    std::string totalInteresesxPagar;
    std::string montoInicial;
    std::string porcientoInicialFormato;

    while (true) { // Bucle infinito para mostrar el men�
        std::cout << "1. Calcular Monto de Cuotas de un Pr�stamo\n";
        std::cout << "2. Calcular Tasa de Inter�s\n";
        std::cout << "3. Salir\n";
        opcion = ObtenerEntero("Elige una opci�n: ", 1, 3);

        if (opcion == 3) break; // Salir del men�

        switch (opcion) {
        case 1:
            capital = ObtenerDecimal("\nIngrese el monto del pr�stamo (mayor a 1,000.00): ", 1000, std::numeric_limits<double>::max());
            tasaInteres = ObtenerDecimal("Ingrese el inter�s (% anual) (mayor a 1.00): ", 1, 50);
            anos = ObtenerEntero("Ingrese el tiempo (a�os) (mayor a 1): ", 1, 20);
            totalIntereses = 0;
            restanteCapital = 0;
            calculoInicial = 0;

            std::cout << "\n�Contempla un pago inicial?\n";
            std::cout << "1. S�\n";
            std::cout << "Otro. No\n";
            std::cout << "Elige una opci�n: ";
            std::cin >> inicialOpcion;

            switch (inicialOpcion) {
            case 1:
                inicial = true;
                porcientoInicial = ObtenerEntero("\nIngrese el porcentaje del inicial: ", 1, 99);

                calculoInicial = capital * (static_cast<double>(porcientoInicial) / 100.0);
                montoInicial = FormatearNumero(calculoInicial);
                porcientoInicialFormato = FormatearNumero(porcientoInicial);

                capital -= calculoInicial;

                printf("\nPorcentaje de inicial: %s%%\n", porcientoInicialFormato.c_str());
                printf("\nMonto de inicial a pagar: %s\n", montoInicial.c_str());

            default:
                totalIntereses = 0;
                restanteCapital = 0;
                calculoInicial = 0;

                inicial = false;
            }

            cuota = CalcularMontoCuota(capital, tasaInteres, anos);
            cuotaFormateada = FormatearNumero(cuota);

            totalIntereses = calculoIntereses(capital, tasaInteres, anos, cuota);

            totalInteresesxPagar = FormatearNumero(totalIntereses);
            printf("\nMonto de la cuota mensual: %s\n", cuotaFormateada.c_str());
            printf("\nMonto total de Intereses a Pagar: %s\n", totalInteresesxPagar.c_str());



            opcion = ObtenerEntero("\n�Desea ver la tabla de amortizaci�n? (1 para s�, 2 para no): ", 1, 2);
            if (opcion == 1) {
                GenerarTablaAmortizacion(capital, tasaInteres, anos, cuota);

                std::cout << "\nPresione Enter para continuar...\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la l�nea
                std::cin.get(); // Espera a que el usuario presione Enter
            }
            break;

        case 2:
            capital = ObtenerDecimal("\nIngrese el monto del pr�stamo: ", 1000, std::numeric_limits<double>::max());
            cuota = ObtenerDecimal("Ingrese el monto de la cuota: ", 1, std::numeric_limits<double>::max());
            anos = ObtenerEntero("Ingrese el tiempo (a�os): ", 1, 20);

            tasaInteres = CalcularTasaInteres(capital, cuota, anos);
            // Si la tasa de inter�s es v�lida, mostrarla
            if (tasaInteres != -1) {
                std::cout << "Tasa de inter�s: " << std::fixed << std::setprecision(2) << tasaInteres << "%" << std::endl;
            }

            std::cout << "\nPresione Enter para continuar...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora el resto de la l�nea
            std::cin.get(); // Espera a que el usuario presione Enter
            break;

        default:
            std::cout << "Opci�n no v�lida. Intente de nuevo." << std::endl;
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
