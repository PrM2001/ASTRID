from skyfield.api import load
import time
# Create a timescale and ask the current time.

start_time = time.perf_counter()

ts = load.timescale()
t = ts.now()

# Load the JPL ephemeris DE421 (covers 1900-2050).
planets = load('de421.bsp')

earth, venus = planets['earth'], planets['venus barycenter']


# What's the position of Mars, viewed from Earth?
astrometric = earth.at(t).observe(venus).apparent()
ra, dec, distance = astrometric.radec()

print("Vector from Earth to Uranus")
print("Right ascension: " + str(ra))
print("Declination: " + str(dec))
print("Distance from Earth: " + str(distance))

end_time = time.perf_counter()
print(end_time - start_time)
# # Load the ephemeris data for the planets and Earth
# eph = load('de421.bsp')
# earth = eph['earth']

# # Set the observer location to the desired latitude and longitude
# observer = Topos(latitude_degrees=37.8806, longitude_degrees=-122.2448)

# # Get the position of Mars as seen from Earth at the current time
# mars = eph['mars']
# ts = load.timescale()
# t = ts.now()
# pos = (earth).at(t).observe(mars)

# # Compute the right ascension and declination of Mars
# ra, dec, _ = pos.radec()

# # Print the results
# print('Right Ascension:', ra._degrees)
# print('Declination:', dec)