from WebsterFDTDReadWrite import WebsterFDTDReadWrite
import numpy as np
import matplotlib.pyplot as plt

# Change the build directory here if needed
runner = WebsterFDTDReadWrite(
    "../build/examples/WebsterFDTDReadWrite/phonos-websterFDTDReadWrite")

# Simulation setting


runner.duration = 1
runner.l0 = 17e-2
runner.articulation_mode = 0  # Set to constant section
runner.constant_section = np.pi * 1e-4

Amp = 1e-4
width = 1e-4


def window(t):
    return np.hanning(len(t))


def Qin(t):
    # out = np.zeros_like(t)
    # out[0] = 1e-4
    # out = (np.random.random(len(t)) - 0.5) * 2 * Amp
    # return out * window(t)
    return Amp * np.sin(np.pi * t / width) * (t < width)


runner.Qin = Qin


runner.store_spatial_distributions = True
runner.compute_powers = True

mute = False

# With walls and radiation
runner.yielding_walls = True
runner.radiation = True
results = runner.run_simulation("ImpedanceExample.hdf5", mute)

freqs = np.fft.rfftfreq(len(runner.t), d=1.0 / runner.sr)
fft_Qin = np.fft.rfft(Qin(runner.t))
fft_Pin = np.fft.rfft(results["inputPressure"])

Z = fft_Pin / fft_Qin
Z0 = results["rho0"] * results["c0"] / runner.constant_section
Z_rel = Z / Z0

fig, axs = plt.subplots(2, 1, sharex=True)
axs[0].plot(freqs, 20 * np.log10(np.abs(Z_rel)), label="Walls and radiation")
axs[1].plot(freqs, np.angle(Z_rel))

# With walls and radiation
runner.yielding_walls = True
runner.radiation = True
results = runner.run_simulation("ImpedanceExample.hdf5", mute)

freqs = np.fft.rfftfreq(len(runner.t), d=1.0 / runner.sr)
fft_Qin = np.fft.rfft(Qin(runner.t))
fft_Pin = np.fft.rfft(results["inputPressure"])

Z = fft_Pin / fft_Qin
Z_rel = Z / Z0

axs[0].plot(freqs, 20 * np.log10(np.abs(Z_rel)),
            label="No walls and radiation")
axs[1].plot(freqs, np.angle(Z_rel))

# With no walls and no radiation
runner.yielding_walls = False
runner.radiation = False
results = runner.run_simulation("ImpedanceExample.hdf5", mute)

freqs = np.fft.rfftfreq(len(runner.t), d=1.0 / runner.sr)
fft_Qin = np.fft.rfft(Qin(runner.t))
fft_Pin = np.fft.rfft(results["inputPressure"])

Z = fft_Pin / fft_Qin
Z_rel = Z / Z0

axs[0].plot(freqs, 20 * np.log10(np.abs(Z_rel)),
            label="No walls and no radiation")
axs[1].plot(freqs, np.angle(Z_rel))


axs[0].set_ylabel(r"$20 \ {log}(abs(Z_{rel}))$")
axs[1].set_ylabel("Angle")
axs[1].set_xlabel("Frequency [Hz]")
axs[1].set_yticks([-np.pi/2, 0,  np.pi/2], [r"$-\pi/2$", 0, r"$\pi/2$"])

axs[0].set_ylim([-40, 40])
axs[0].set_xlim([freqs[0], 5000])
axs[0].grid()
axs[1].grid()

fig.legend()
fig.suptitle("Simulated impedance")

plt.show()
