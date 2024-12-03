# QIX 🖥️

## 📄 Descrição
QIX é um clássico jogo arcade adaptado para a linguagem C. Neste jogo, o jogador controla uma "pá" que deve desenhar linhas em uma área para capturar território, ao mesmo tempo em que evita inimigos que se movem aleatoriamente pela tela. O objetivo é capturar a maior parte do território, dividindo-o em zonas e evitando os obstáculos.

## 🎲 Objetivo do Jogo
O objetivo de QIX é capturar uma porcentagem específica da área da tela para completar cada nível. Para isso, você precisa "cortar" o território desenhando linhas e fechando áreas, mas cuidado com os inimigos que rondam o campo, pois eles destroem suas linhas em progresso. Completar a área necessária o mais rápido possível é o segredo para obter a maior pontuação.

## ⚔️ Inimigos
No jogo QIX, há dois tipos principais de inimigos:
- **Qix:** A entidade principal que se move aleatoriamente na tela. É um inimigo imprevisível e letal; evite qualquer contato com ele enquanto desenha.
- **Sparx:** Pequenos inimigos que percorrem as bordas da tela e podem destruir sua "pá" se encontrarem sua linha. Eles aumentam em número conforme você avança nos níveis.

## 👾 Pontuação
A Scoreboard é baseada no tempo do usuário, conforme menor o tempo, maior a posiçãp na tabela.

## ⚙️ Compilação - Exemplo
```bash
cd /mnt/c/Users/usuario/Downloads/faculdade/Qix/cli-lib
gcc ./src/main.c $(find ./src -name "*.c" ! -name "main.c") -I./include -o qix
./qix
```
## 👩‍💻 Membros
Este projeto foi desenvolvido com o esforço colaborativo dos seguintes membros:
- **[Arthur Xavier](https://github.com/arthurxavi)** 
- **[Antônio Laprovitera](https://github.com/antoniolaprov)** 
- **[Guilherme Vinícius](https://github.com/GuilhermeRangel1)**


