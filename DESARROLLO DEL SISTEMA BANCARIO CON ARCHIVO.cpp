git clone <URL-del-repositorio>
cd <nombre-del-repositorio>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // Para ocultar la contraseña (getch)
//Hola
#define MAX_CUENTAS 5
#define MAX_INTENTOS 3
#define MAX_CARACTERES_CUENTA 9
#define ARCHIVO_USUARIOS "bancaUsuarios.txt"

// Estructura para las cuentas
typedef struct {
	int numero_cuenta;
	char contrasena[15];
	float saldo;
	float transacciones[100];
	int num_transacciones;
} Cuenta;

// Declaración de las cuentas
Cuenta cuentas[MAX_CUENTAS] = {
	{202411263, "Abigail01", 0.0, {0}, 0},
{202410494, "Azaela02", 0.0, {0}, 0},
	{202411006, "Deysi03", 0.0, {0}, 0},
{202411229, "Brenthon04", 0.0, {0}, 0},
	{202410331, "Jhon05", 0.0, {0}, 0}
};

// Función para limpiar la pantalla
void limpiar_pantalla() {
	system("cls");
}

// Función para validar entrada de un número de cuenta
bool validar_numero_cuenta(const char *entrada) {
	if (strlen(entrada) > MAX_CARACTERES_CUENTA) return false;
	for (int i = 0; entrada[i] != '\0'; i++) {
		if (entrada[i] < '0' || entrada[i] > '9') return false;
	}
	return true;
}

// Función para ocultar la contraseña
void leer_contrasena(char *contrasena) {
	char c;
	int i = 0;
	while ((c = getch()) != '\n' && c != '\r') {
		if (c == '\b' && i > 0) { // Manejo de retroceso
			printf("\b \b");
			i--;
		} else if (c != '\b' && i < 14) {
			contrasena[i++] = c;
			printf("*");
		}
	}
	contrasena[i] = '\0';
}

// Función para encontrar una cuenta por número (transferencias)
int buscar_cuenta(int numero_cuenta) {
	for (int i = 0; i < MAX_CUENTAS; i++) {
		if (cuentas[i].numero_cuenta == numero_cuenta) {
			return i;
		}
	}
	return -1;
}

// Función para cargar saldos desde el archivo
void cargar_saldos() {
	FILE *archivo = fopen(ARCHIVO_USUARIOS, "r");
	if (archivo == NULL) {
		printf("\nNo se encontro el archivo '%s'. Se inicializan los saldos a 0.\n", ARCHIVO_USUARIOS);
		return;
	}
	
	for (int i = 0; i < MAX_CUENTAS; i++) {
		if (fscanf(archivo, "%f", &cuentas[i].saldo) != 1) {
			cuentas[i].saldo = 0.0;
		}
	}
	
	fclose(archivo);
	printf("\nSaldos cargados exitosamente desde '%s'.\n", ARCHIVO_USUARIOS);
}

// Función para guardar saldos en el archivo
void guardar_saldos() {
	FILE *archivo = fopen(ARCHIVO_USUARIOS, "w");
	if (archivo == NULL) {
		printf("\nError al guardar en el archivo '%s'.\n", ARCHIVO_USUARIOS);
		return;
	}
	
	for (int i = 0; i < MAX_CUENTAS; i++) {
		fprintf(archivo, "%.2f\n", cuentas[i].saldo);
	}
	
	fclose(archivo);
	printf("\nSaldos guardados exitosamente en '%s'.\n", ARCHIVO_USUARIOS);
}

// Función para realizar un depósito
void realizar_deposito(Cuenta *cuenta) {
	float monto;
	char entrada[20];
	while (true) {
		printf("\nIngrese el monto a depositar (formato decimal con coma, ej.: 100,25): ");
		scanf("%s", entrada);
		
		// Validar formato y convertir
		char *coma = strchr(entrada, ',');
		if (coma) {
			*coma = '.'; // Sustituir coma por punto para atof
			monto = atof(entrada);
			
			if (monto >= 0.01 && monto <= 10000.00) {
				cuenta->saldo += monto;
				cuenta->transacciones[cuenta->num_transacciones++] = monto;
				limpiar_pantalla();
				printf("\nDeposito exitoso. Nuevo saldo: %.2f\n", cuenta->saldo);
				guardar_saldos();
				break;
			}
		}
		limpiar_pantalla();
		printf("\nMonto invalido. Intente nuevamente.\n");
	}
}

// Función para realizar un retiro
void realizar_retiro(Cuenta *cuenta) {
	int opcion;
	float monto;
	char entrada[10];
	do {
		printf("\nSeleccione una cantidad a retirar:\n");
		printf("1. $10\n");
		printf("2. $20\n");
		printf("3. $50\n");
		printf("4. $100\n");
		printf("5. Otro monto\n");
		printf("Opcion: ");
		scanf("%s", entrada);
		
		if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '5') {
			opcion = entrada[0] - '0';
		} else {
			limpiar_pantalla();
			printf("\nOpcion invalida. Intente nuevamente.\n");
			continue;
		}
		
		switch (opcion) {
		case 1: monto = 10; break;
		case 2: monto = 20; break;
		case 3: monto = 50; break;
		case 4: monto = 100; break;
		case 5:
			printf("\nIngrese el monto a retirar (en multiplos de 10, hasta $500): ");
			scanf("%f", &monto);
			if ((int)monto % 10 != 0 || monto <= 0 || monto > 500) {
				limpiar_pantalla();
				printf("\nMonto invalido. Intente nuevamente.\n");
				monto = -1; // Indica monto invalido
			}
			break;
		}
		
		if (monto > 0 && cuenta->saldo >= monto) {
			cuenta->saldo -= monto;
			cuenta->transacciones[cuenta->num_transacciones++] = -monto;
			limpiar_pantalla();
			printf("\nRetiro exitoso. Monto retirado: %.2f. Nuevo saldo: %.2f\n", monto, cuenta->saldo);
			guardar_saldos();
			break;
		} else if (monto > 0) {
			limpiar_pantalla();
			printf("\nSaldo insuficiente para realizar el retiro.\n");
		}
	} while (true);
}

// Función para realizar una transferencia
void realizar_transferencia(Cuenta *cuenta) {
	int numero_destino;
	float monto;
	char entrada[20];
	Cuenta cuenta_externa = {0, "", 0.0, {0}, 0};
	Cuenta *cuenta_destino;
	char entrada_cuenta[20];
	
	while (true) {
		printf("\nIngrese el numero de cuenta destino: ");
		scanf("%s", entrada_cuenta);
		
		if (!validar_numero_cuenta(entrada_cuenta)) {
			limpiar_pantalla();
			printf("\nNumero de cuenta invalido. Intente nuevamente.\n");
			continue;
		}
		
		numero_destino = atoi(entrada_cuenta);
		int indice_destino = buscar_cuenta(numero_destino);
		
		if (indice_destino == -1) {
			printf("\nCuenta cotejada.\n");
			cuenta_externa.numero_cuenta = numero_destino;
			cuenta_destino = &cuenta_externa;
		} else {
			cuenta_destino = &cuentas[indice_destino];
		}
		break;
	}
	
	while (true) {
		printf("\nIngrese el monto a transferir (formato decimal con coma, ej.: 100,25): ");
		scanf("%s", entrada);
		
		// Validar formato y convertir
		char *coma = strchr(entrada, ',');
		if (coma) {
			*coma = '.'; // Sustituir coma por punto para atof
			monto = atof(entrada);
			
			if (monto >= 0.01 && monto <= 10000.00) {
				if (cuenta->saldo >= monto) {
					cuenta->saldo -= monto;
					cuenta_destino->saldo += monto;
					cuenta->transacciones[cuenta->num_transacciones++] = -monto;
					limpiar_pantalla();
					printf("\nTransferencia exitosa. Monto transferido: %.2f. Nuevo saldo: %.2f\n", monto, cuenta->saldo);
					guardar_saldos();
				} else {
					limpiar_pantalla();
					printf("\nSaldo insuficiente para realizar la transferencia.\n");
				}
				break;
			}
		}
		limpiar_pantalla();
		printf("\nMonto invalido. Intente nuevamente.\n");
	}
}

// Función para mostrar el historial de transacciones
void mostrar_transacciones(Cuenta *cuenta) {
	printf("\nHistorial de transacciones:\n");
	if (cuenta->num_transacciones == 0) {
		printf("No hay transacciones realizadas.\n");
	} else {
		for (int i = 0; i < cuenta->num_transacciones; i++) {
			printf("%s %.2f\n", cuenta->transacciones[i] > 0 ? "Deposito" : "Retiro/Transferencia", cuenta->transacciones[i]);
		}
	}
}

// Menú principal
void menu_principal(Cuenta *cuenta) {
	int opcion;
	char entrada[10];
	do {
		printf("\nBienvenido al sistema bancario. Seleccione una opcion:\n");
		printf("1. Consultar saldo\n");
		printf("2. Realizar un deposito\n");
		printf("3. Realizar un retiro\n");
		printf("4. Realizar una transferencia\n");
		printf("5. Ver historial de transacciones\n");
		printf("6. Salir\n");
		printf("Opcion: ");
		scanf("%s", entrada);
		
		if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '6') {
			opcion = entrada[0] - '0';
		} else {
			opcion = -1;
		}
		
		limpiar_pantalla();
		switch (opcion) {
		case 1:
			printf("\nSu saldo actual es: %.2f\n", cuenta->saldo);
			break;
		case 2:
			realizar_deposito(cuenta);
			break;
		case 3:
			realizar_retiro(cuenta);
			break;
		case 4:
			realizar_transferencia(cuenta);
			break;
		case 5:
			mostrar_transacciones(cuenta);
			break;
		case 6:
			printf("\nGracias por usar el sistema bancario. Hasta luego.\n");
			break;
		default:
			printf("\nOpcion invalida. Intente nuevamente.\n");
		}
	} while (opcion != 6);
}

// Función principal
int main() {
	int numero_cuenta;
	char contrasena[15];
	int intentos;
	int indice_cuenta;
	
	limpiar_pantalla();
	printf("Bienvenido al sistema bancario.\n\n");
	
	cargar_saldos();
	
	printf("Ingrese su numero de cuenta: ");
	scanf("%d", &numero_cuenta);
	
	indice_cuenta = buscar_cuenta(numero_cuenta);
	if (indice_cuenta == -1) {
		printf("\nCuenta no registrada. Acceso denegado.\n");
		return 0;
	}
	
	intentos = 0;
	while (intentos < MAX_INTENTOS) {
		printf("\nIntentos restantes: %d\n", MAX_INTENTOS - intentos);
		printf("Ingrese su contrasena: ");
		leer_contrasena(contrasena);
		
		if (strcmp(cuentas[indice_cuenta].contrasena, contrasena) == 0) {
			limpiar_pantalla();
			menu_principal(&cuentas[indice_cuenta]);
			return 0;
		} else {
			printf("\nContrasena incorrecta. Intente nuevamente.\n");
			intentos++;
		}
	}
	
	printf("\nHa excedido el numero maximo de intentos. Acceso denegado.\n");
	return 0;
}
