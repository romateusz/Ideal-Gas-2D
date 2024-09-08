import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patches as patches

# Funkcja do wczytywania danych z pliku
def load_data(filename):
    data = np.loadtxt(filename, delimiter=',')
    positions = data[:, :2]
    velocities = data[:, 2:]
    return positions, velocities

# Funkcja do obliczania energii kinetycznej
def compute_kinetic_energy(velocities):
    return 0.5 * np.sum(np.linalg.norm(velocities, axis=1)**2)

# Funkcja do obliczania pędu
def compute_momentum(velocities):
    return np.sum(velocities, axis=0)

# Funkcja rysująca wykresy
def update_plot(frame, all_positions, all_velocities, energy_plot, momentum_plot, l, r):
    plt.clf()

    # Pobierz pozycje i prędkości dla aktualnej klatki
    positions = all_positions[frame]
    velocities = all_velocities[frame]

    # Rysowanie pozycji cząsteczek
    ax1 = plt.subplot(2, 2, 1)
    ax1.set_xlim(0, l)
    ax1.set_ylim(0, l)
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_title('Aktualne pozycje cząsteczek')
    
    # Dodaj okręgi dla cząsteczek
    for pos in positions:
        circle = patches.Circle(pos, r, color='blue', fill=True)
        ax1.add_patch(circle)
    
    # Ustaw proporcje osi na równe
    ax1.set_aspect('equal', adjustable='box')

    # Rysowanie histogramu prędkości
    plt.subplot(2, 2, 2)
    speeds = np.linalg.norm(velocities, axis=1)
    plt.hist(speeds, bins=30, color='orange', edgecolor='black')
    plt.title('Histogram prędkości')
    plt.xlabel('Prędkość')
    plt.ylabel('Liczba cząsteczek')
    
    # Rysowanie energii kinetycznej
    kinetic_energy = compute_kinetic_energy(velocities)
    energy_plot.append(kinetic_energy)
    time = np.arange(len(energy_plot)) * dt
    plt.subplot(2, 2, 3)
    plt.plot(time, energy_plot, color='green')
    plt.title('Energia kinetyczna')
    plt.xlabel('Czas')
    plt.ylabel('Energia')
    
    # Rysowanie pędu układu
    momentum = compute_momentum(velocities)
    momentum_plot.append(np.linalg.norm(momentum))
    plt.subplot(2, 2, 4)
    plt.plot(time, momentum_plot, color='red')
    plt.title('Pęd układu')
    plt.xlabel('Czas')
    plt.ylabel('Pęd')
    
    plt.tight_layout()

# Parametry
with open("visual.config", "r") as f:
    lines = f.readlines()
n = int(lines[0].strip().split(",")[1])
l = float(lines[1].strip().split(",")[1])
r = float(lines[2].strip().split(",")[1])
dt = float(lines[3].strip().split(",")[1])
s = int(lines[4].strip().split(",")[1])

file_count = s
filenames = [f'particles_{i}.txt' for i in range(file_count)]

# Wczytanie wszystkich danych na początku do numpy arrays
all_positions = np.zeros((file_count, n, 2))
all_velocities = np.zeros((file_count, n, 2))

for i, filename in enumerate(filenames):
    positions, velocities = load_data(filename)
    all_positions[i] = positions
    all_velocities[i] = velocities

# Inicjalizacja wykresów
energy_plot = []
momentum_plot = []

fig = plt.figure(figsize=(14, 10))
ani = FuncAnimation(fig, update_plot, frames=file_count, fargs=(all_positions, all_velocities, energy_plot, momentum_plot, l, r), interval=50)

plt.show()