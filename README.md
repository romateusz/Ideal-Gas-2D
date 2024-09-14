# Projekt "Symulacja Gazu 2D"

Ten projekt symuluje ruch cząsteczek dwuwymiarowego gazu doskonałego poruszających się w zamkniętej przestrzeni oraz analizuje właściwości układu, takie jak energia kinetyczna cząsteczek i pęd układu. Wizualizacja jest realizowana przy użyciu animacji w `matplotlib`.

## Wymagania
- Kompilator C++
- Python 3
- Biblioteki Python: `numpy`, `matplotlib`

## Pliki projektu

- particless.h - plik nagłówkowy zawierający definicję klas particle i particles.
  
- particles.cpp - plik zawierający implementacje powyższych klas.
- main.cpp - plik korzystający z napisanych wcześniej plików w celu przeprowadzenia symulacji zachowania gazu doskonałego 2D. Automatycznie generuje plik "visual.config" oraz kolejne pliki zawierające obliczone dane z kolejnych kroków czasowych.
  
- visual.config - plik konfiguracyjny, generowany automatycznie po uruchomieniu programu wygenerowanego po kompilacji, który zawiera następujące parametry symulacji:

n - Liczba cząsteczek.
l - Długość boku planszy (kwadratowa plansza).
r - Promień cząsteczek.
dt - Krok czasowy dla animacji.
s - Liczba plików danych.

- particles_{i}.txt - Pliki danych cząsteczek, gdzie {i} to numer pliku. Każdy plik zawiera dane cząsteczek w formacie:

- visualize.py - Skrypt Python do wizualizacji danych. Używa matplotlib do rysowania animacji i wykresów.

## Uruchomienie

- Skompiluj program na swojej stacji roboczej, za pomocą poniższej instrukcji. (rekomendowany standard to c++20)
  >> g++ -std=c++20 main.cpp particles.cpp -o symulacja
- po poprawnym skompilowaniu programu uruchom utworzony program "symulacja.exe".
  >> ./symulacja
- Uruchom skrypt do wizualizacji: Aby uruchomić animację, uruchom skrypt visualisation.py:
  >> python3 visualisation.py

Projekt został stworzony przez Mateusza Roman.
