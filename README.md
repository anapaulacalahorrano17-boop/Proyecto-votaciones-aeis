# Proyecto: Sistema de Votaciones Electrónicas
**Estudiantes:** Antonela Brito, Ana Calahorrano, Sebastian Salvatierra 

**Institución:** Escuela Politécnica Nacional 

Este es un programa desarrollado en lenguaje C que simula un sistema de votaciones interactivo. Permite gestionar el registro de votos, calcular estadísticas en tiempo real y determinar al candidato ganador de manera automatizada y eficiente.

## 🚀 Características principales

*   Registro de Votantes: Control y validación para asegurar que cada voto sea único.
*   Conteo Automatizado: Procesamiento de datos mediante estructuras de control para acumular votos por candidato, votos en blanco y votos nulos.
*   Estadísticas Detalladas: Cálculo de porcentajes de participación y resultados para cada opción.
*   Reporte de Resultados: Despliegue en pantalla del ganador absoluto.

## 🔩 Estructura del proyecto
El proyecto se encuentra organizado bajo los siguientes componentes fundamentales:
*   *PB2.c*: Archivo de código fuente principal. Contiene la definición de estructuras (Candidato y Votante), el menú interactivo, la lógica de validación de votantes y las funciones de ordenación por selección para el escrutinio.
*   *padron.txt*: Archivo de texto que funciona como base de datos local, almacenando la lista de estudiantes habilitados para ejercer su derecho al voto.
*   *resultados.csv*: Archivo de salida estructurado generado automáticamente que registra de forma ordenada el conteo de votos finales.
*   *Reporte_Electoral.pdf*: Documento generado por el sistema que consolida el acta oficial de resultados, estadísticas de participación y el candidato electo.

## 📦 Cómo ejecutar el programa

1. Clona este repositorio o descarga el archivo fuente.
2. Abre el archivo en tu entorno de desarrollo. Por ejemplo: VS Code
3. Compila y ejecuta el código.
4. Sigue las instrucciones en la consola para ingresar los votos.
