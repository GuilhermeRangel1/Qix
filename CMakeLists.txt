cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(QixProject)

# Definir o padrão de compilação
set(CMAKE_C_STANDARD 11)

# Incluir diretórios com cabeçalhos
include_directories(cli-lib/include)

# Adiciona os arquivos fonte
set(SOURCES
        cli-lib/src/screen.c
        cli-lib/src/keyboard.c
        cli-lib/src/timer.c
        src/main.c  # Adicione o main.c aqui
)

# Cria um executável
add_executable(QixExecutable ${SOURCES})
