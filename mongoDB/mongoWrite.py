# login met Write:
# mongo -u "write" -p "Reteip@1" --authenticationDatabase "solarTrackerDB"

# login met Read:
# mongo -u "readOnly" -p "reteip" --authenticationDatabase "solarTrackerDB"

from pymongo import MongoClient
from urllib.parse import quote_plus

#save current locahostIp to variable
import socket
localhost = socket.gethostbyname(socket.gethostname())
print("localhost:", localhost)

# Inloggegevens en verbindingsdetails
# ip = 'localhost'
ip = '192.168.0.205'
port = 27017

username = "write"
password = "Reteip@1"
database_name = "solarTrackerDB"

# Codeer de gebruikersnaam en het wachtwoord
encoded_username = quote_plus(username)
encoded_password = quote_plus(password)

# Verbind met MongoDB
# uri = f"mongodb://{encoded_username}:{encoded_password}@localhost:27017/{database_name}"
uri = f"mongodb://{encoded_username}:{encoded_password}@{ip}:{port}/{database_name}"
client = MongoClient(uri)

# Print client info
print("MongoDB Client Info:")
print(f"Client: {client}")
print(f"Connected to: {client.address}")
print(f"Server Version: {client.server_info()['version']}")
print(f"MongoDB URI: {uri}")
exit()

# Print alle databases
print("\nDatabases:")
for db_name in client.list_database_names():
    print(f"- {db_name}")



# Print collecties in de geselecteerde database
db = client[database_name]
print(f"\nCollecties in {database_name}:")
for collection_name in db.list_collection_names():
    print(f"- {collection_name}")

    # Print alle records in de collectie
    collection = db[collection_name]
    print(f"Records in de collectie '{collection_name}':")
    for record in collection.find():
        print(record)


# Print de stats van de database
stats = db.command("dbStats")
print(f"Database '{database_name}' Info:")
print(f"  - Data Size: {stats['dataSize']} bytes")
print(f"  - Storage Size: {stats['storageSize']} bytes")
print(f"  - Index Size: {stats['indexSize']} bytes")
print(f"  - Total Size: {stats['dataSize'] + stats['indexSize']} bytes")
print(f"  - Collections Count: {stats['collections']}")
print(f"  - Objects Count: {stats['objects']}")
print(f"  - Indexes Count: {stats['indexes']}")
print(f"  - Avg Object Size: {stats['avgObjSize']} bytes")



# Sluit de verbinding
client.close()