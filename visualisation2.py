import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patches as patches
from matplotlib.collections import PatchCollection

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

# Utworzenie figure i osi
fig, axs = plt.subplots(2, 2, figsize=(14, 10))
ax1, ax2, ax3, ax4 = axs.flatten()

# Inicjalizacja dla pozycji cząsteczek
ax1.set_xlim(0, l)
ax1.set_ylim(0, l)
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_title('Aktualne pozycje cząsteczek')
ax1.set_aspect('equal', adjustable='box')

# Inicjalizacja dla okręgów
circles = [patches.Circle((0, 0), r, color='blue') for _ in range(n)]
patch_collection = PatchCollection(circles, match_original=True)
ax1.add_collection(patch_collection)

# Inicjalizacja dla histogramu prędkości
ax2.set_title('Histogram prędkości')
ax2.set_xlabel('Prędkość')
ax2.set_ylabel('Liczba cząsteczek')

# Inicjalizacja dla energii kinetycznej
ax3.set_title('Energia kinetyczna')
ax3.set_xlabel('Czas')
ax3.set_ylabel('Energia')

# Inicjalizacja dla pędu układu
ax4.set_title('Pęd układu')
ax4.set_xlabel('Czas')
ax4.set_ylabel('Pęd')

# Inicjalizacja linii do wykresów
line_energy, = ax3.plot([], [], color='green')
line_momentum, = ax4.plot([], [], color='red')

# Funkcja aktualizująca
def update_plot(frame):
    # Aktualizacja pozycji cząsteczek
    positions = all_positions[frame]
    velocities = all_velocities[frame]

    # Aktualizacja okręgów - zmiana położenia
    for circle, pos in zip(circles, positions):
        circle.center = pos

    patch_collection.set_paths(circles)  # Aktualizacja PatchCollection

    # Aktualizacja histogramu prędkości
    ax2.clear()
    ax2.set_title('Histogram prędkości')
    ax2.set_xlabel('Prędkość')
    ax2.set_ylabel('Liczba cząsteczek')
    speeds = np.linalg.norm(velocities, axis=1)
    ax2.hist(speeds, bins=30, color='orange', edgecolor='black')

    # Aktualizacja energii kinetycznej
    kinetic_energy = compute_kinetic_energy(velocities)
    energy_plot.append(kinetic_energy)
    time = np.arange(len(energy_plot)) * dt
    line_energy.set_data(time, energy_plot)
    ax3.relim()
    ax3.autoscale_view()

    # Aktualizacja pędu układu
    momentum = compute_momentum(velocities)
    momentum_plot.append(np.linalg.norm(momentum))
    line_momentum.set_data(time, momentum_plot)
    ax4.relim()
    ax4.autoscale_view()

# Animacja
ani = FuncAnimation(fig, update_plot, frames=file_count, interval=100)

# Dopasowanie layoutu
plt.tight_layout(pad=2)
plt.show()