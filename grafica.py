import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Generar datos de caminata para 50 pasos (200 valores: 4 por paso)
num_steps = 50

# Valores específicos para cada paso: máxima plantiflexión y máxima dorsiflexión
plantarflexion_values = np.random.uniform(-35, -30, num_steps).astype(int)  # Máximo ángulo hacia abajo (negativo)
dorsiflexion_values = np.random.uniform(15, 20, num_steps).astype(int)      # Máximo ángulo hacia arriba
neutral_values = np.zeros(num_steps).astype(int)                            # Posición neutral

# Calcular los valores intermedios como la suma del valor absoluto de plantiflexión y dorsiflexión
intermediate_values = (np.abs(plantarflexion_values) + dorsiflexion_values).astype(int)

# Repetir los valores para obtener el patrón de 4 valores por paso
all_ankle_angles = np.array([neutral_values, plantarflexion_values, dorsiflexion_values, neutral_values]).T

# Promediar los valores de los 50 pasos para cada frame
average_ankle_angles = np.mean(all_ankle_angles, axis=0).astype(int)

# Crear DataFrame
data_corrected_steps = {
    "Frame": np.arange(1, len(average_ankle_angles) + 1),
    "Ankle_Angle": average_ankle_angles
}

df_corrected_steps = pd.DataFrame(data_corrected_steps)

# Mostrar el DataFrame en la terminal
print(df_corrected_steps)

# Encontrar los valores máximo y mínimo
max_value = df_corrected_steps['Ankle_Angle'].max()
max_index = df_corrected_steps['Ankle_Angle'].idxmax()
min_value = df_corrected_steps['Ankle_Angle'].min()
min_index = df_corrected_steps['Ankle_Angle'].idxmin()

# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(df_corrected_steps["Frame"], df_corrected_steps["Ankle_Angle"], linestyle='-', color='green', marker='o', label='Average Ankle Angle')

# Mejorar la gráfica
plt.fill_between(df_corrected_steps["Frame"], df_corrected_steps["Ankle_Angle"], color='green', alpha=0.1)
plt.axhline(y=0, color='black', linestyle='--', linewidth=0.8)
plt.axvline(x=2.5, color='red', linestyle='--', linewidth=0.8, label='Mid Step')

# Añadir etiquetas y título
plt.title("Average Ankle Joint Angles During Walking (50 Steps)", fontsize=14)
plt.xlabel("Frame", fontsize=12)
plt.ylabel("Angle (°)", fontsize=12)

# Añadir indicadores de valor máximo y mínimo sin decimales
plt.annotate(f'Max: {int(max_value)}', xy=(max_index + 1, max_value), xytext=(max_index + 2, max_value + 10),
             arrowprops=dict(facecolor='black', arrowstyle="->"))
plt.annotate(f'Min: {int(min_value)}', xy=(min_index + 1, min_value), xytext=(min_index + 2, min_value - 10),
             arrowprops=dict(facecolor='black', arrowstyle="->"))

# Añadir cuadrícula y leyenda
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend(loc='upper right')

# Mostrar la gráfica
plt.show()
