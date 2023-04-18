from skyfield.api import load

# Create a timescale and ask the current time.
ts = load.timescale()
t = ts.now()

# Load the JPL ephemeris DE421 (covers 1900-2050).
planets = load('de421.bsp')
#moons = load('jup365.bsp')
print(planets)
earth, jupiter = planets['earth'], planets['jupiter barycenter']
#ganymede = moons['ganymede']

# What's the position of Mars, viewed from Earth?
astrometric = earth.at(t).observe(jupiter).apparent()
ra, dec, distance = astrometric.radec()

print("Vector from Earth to Jupiter")
print("Right ascension: " + str(ra._degrees))
print("Declination: " + str(dec._degrees))
print("Distance from Earth: " + str(distance))