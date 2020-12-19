/*
Штирлиц и Мюллер стреляют по очереди. В очереди n человек, стоящих друг за другом. Каждым выстрелом убивается один из стоящих.
Кроме того, если у кого-то из стоящих в очереди убиты все его соседи, то этот человек в ужасе убегает. Проигрывает тот, кто не может ходить.
Первым стреляет Штирлиц.
Требуется определить, кто выиграет при оптимальной игре обеих сторон, и если победителем будет Штирлиц, то найти все возможные первые ходы, ведущие к его победе. 
Входной файл содержит единственное число n (2≤ n≤ 5 000) — количество человек в очереди. 
Если выигрывает Мюллер, выходной файл должен состоять из единственного слова Mueller.
Иначе в первой строке необходимо вывести слово Schtirlitz, а в последующих строках — номера людей в очереди, которых мог бы первым ходом убить Штирлиц для достижения своей победы.
Номера необходимо выводить в порядке возрастания. 
*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <optional>

std::optional<std::vector<short>> playGame(short N) {
    std::vector<short> SG(N + 1, 0);
    for (short i = 2; i <= N; ++i) {
        std::unordered_set<short> mex;
        for (short j = 0; j <= (i - 1) / 2; ++j)
            mex.emplace(SG[j] ^ SG[i - j - 1]);
        for (short j = 0; j < i; ++j)
            if (mex.find(j) == mex.end()) {
                SG[i] = j;
                break;
            }
    }
    if (SG[N] == 0)
        return std::nullopt;
    else {
        std::vector<short> firstShot;
        for (short i = 1; i <= N; ++i)
            if ((SG[i - 1] ^ SG[N - i]) == 0)
                firstShot.emplace_back(i);
        return firstShot;
    }
}

int main() {
    short N;
    std::cin >> N;
    if (auto firstShot = playGame(N)) {
        std::cout << "Schtirlitz\n";
        for (auto shot : firstShot.value())
            std::cout << shot << ' ';
    }
    else std::cout << "Mueller\n";
    return 0;
}