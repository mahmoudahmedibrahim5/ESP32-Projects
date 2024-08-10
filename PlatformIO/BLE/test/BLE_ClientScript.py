import asyncio
from bleak import BleakClient

# Replace this with your ESP32's BLE device address
DEVICE_ADDRESS = "EC:62:60:94:BB:A2"

# UUIDs for the service and characteristic you want to access
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

async def connect_and_receive():
    async with BleakClient(DEVICE_ADDRESS) as client:
        print(f"Connected: {client.is_connected}")

        # Reading a characteristic
        try:
            value = await client.read_gatt_char(CHARACTERISTIC_UUID)
            print(f"Received: {value}")
        except Exception as e:
            print(f"Error reading characteristic: {e}")

        # If you want to receive notifications from the characteristic:
        def notification_handler(sender, data):
            print(f"Notification from {sender}: {data}")

        await client.start_notify(CHARACTERISTIC_UUID, notification_handler)

        # Keep the script running to continue receiving notifications
        await asyncio.sleep(30)

        await client.stop_notify(CHARACTERISTIC_UUID)

# Run the event loop
loop = asyncio.get_event_loop()
loop.run_until_complete(connect_and_receive())
