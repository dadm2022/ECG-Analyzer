# ECG Analyzer
## _This repo contains GUI applications for ECG automated signal analysis. App is developed by group of AGH UST students in purpose of course DADM._

## Konfiguracja środowiska

1. Instalacja biblioteki Qt
   1. Uzyskać licencję studencką ze strony (utworzyć konto poprzez konto mailowe w domienie AGH, zaznaczyć 
      opcje: Qt Edu for Developers): https://www.qt.io/qt-educational-license#application
   2. Pobrać instalator poprzez link otrzymany z maila weryfikacyjnego (jeżeli istnieje możliwość to wybrać 
      instalator online).
   3. Postępować zgodnie z instrukcjami:
      W opcji Instalation folder zostawić domyślną ścieżkę. Kolejno zaznaczyć Custom Instalation i nacisnąć Next. 
      Rozwinąć zakładkę Qt i wybrać wersję 5.15 (patch version obojętna, może pojawić się 5.15.7 lub 5.15.11) 
      i wybrać Next. Zaakceptować pliki licencyjne i nacisnąć Next. Przejść przez kolejne zakładki z domyślnymi 
      wartościami klikając Next. Po instalacji odznaczyć wszystkie trzy pola i zamknąć instalator.

2. Klonowanie repozytorium
   1. Przejść do katalogu, w którym checemy mieć projekt
   2. `git clone https://github.com/dadm2022/ECG-Analyzer.git`

3. Dodawanie zmian w kodzie
   1. `git pull origin main`
   2. Przed rozpoczęciem wprowadzania zmian utworzyć nową gałąź
      `git checkout -b [nazwa gałęzi]` - nazwa gałęzi w formacie
      `feature/[nazwa-modułu-oddzielona-myślnikami]`
   3. Zrobić zmiany w kodzie
   4. `git pull origin main`
   5. `git add -A`
   6. `git commit -m "[NAZWA MODUŁU] - [krótki opis zmian]"`
   7. `git push origin [nazwa gałęzi]` w przypadku, gdy jest to kolejny
      commit lub `git push --set-upstream origin [nazwa gałęzi]` w przypadku
      pierwszego commita
   8. Po ostatecznym zakończeniu pracy kliknąć "Create Pull Request"
      w githubie
   9. Zmiana zostanie oceniona przez Architektów i innych członków zespołu
      którzy mogą dodać komentarze/sugestie zmian w kodzie. Nie ma możliwości
      dodania zmian do gałęzi `main` bez akceptacji przynajmniej jednego
      z Architektów.

4. Zmiana ścieżki do [pliku cmake Qt](./CMakeLists.txt)
   1. Odnaleźć linię `set(CMAKE_PREFIX_PATH [ścieżka])`
   2. [Windows] W miejsce `[ścieżka]` wkleić lokalizację podobną do `C:\Qt\5.15.XX\mingw81_64\lib\cmake\Qt5`
      (gdzie `XX` to patch version pobranej biblioteki Qt - może się różnić) <span style="color:red; font-weight:bold;">NIE commitować tego pliku</span>
   3. [MacOS] W miejsce `[ścieżka]` wkleić lokalizację podobną do `/Users/[nazwa_uzytkownika]/Qt/5.15.XX/macos/lib/cmake/Qt5`
      (gdzie `XX` to patch version pobranej biblioteki Qt - może się różnić) <span style="color:red; font-weight:bold;">NIE commitować tego pliku</span>