Tarea Colaborativa (GRUPO 3) En el siguiente documento se explica los pasos que siguió el equipo, los conflictos que se afrontó y como se resolvió.

Pasos:
1. Se identificó e investigó las funcionalidades que necesitaba tener el cajero.
2. Se realizó una tabla de organización RACI.
3. Cada integrante empezó a realizar la parte del programa del cual estaba encargado.
4. Se empezó creando las cuentas y contraseñas que se va a manejar.

Conflictos:
1. Problemas en la estructuración del programa con respecto a las cuentas que se iban a manejar en el cajero. Algunas partes del programa no reconocían correctamente los números de cuenta, lo que generaba errores al realizar transferencias o iniciar sesión.
2. Se tuvo dificultades para sincronizar los datos del programa con un archivo externo. Los cambios realizados en el saldo no siempre se reflejaban en el archivo, lo que provocaba pérdida de datos al cerrar el programa.
3. Al realizar pruebas se detectaron errores en la validación de montos ingresados por los usuarios, especialmente al realizar depósitos y retiros. Algunos montos no válidos, como números negativos o valores no numéricos, causaban fallos en el programa.
4. Al intentar fusionar las ramas de todos los integrantes, hubo conflictos relacionados con funciones duplicadas y diferencias en las funciónes.

Soluciones:
1. Se implementó una estructura con números de cuentas predeterminadas y sus respectivas contraseñas. Además se creo una función de validación para que los números de cuenta sean únicos.
2. Se creó funciones específicas para cargar y guardar saldos en un archivo externo al inicio y al final de la ejecución del programa. También se añadieron validaciones para manejar errores en la apertura del archivo, como su inexistencia o formato incorrecto.
3. Se implementaron funciones de validación para asegurar que los montos ingresados fueran positivos, estuvieran dentro de los límites permitidos y cumplieran con los formatos establecidos.
4. El equipo realizó una revisión conjunta del código, identificando los problemas y unificando las funciones. Además, se rastrearon los cambios y resolvieron los conflictos.
