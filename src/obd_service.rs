use esp32_nimble::{utilities::BleUuid, BLEServer, NimbleProperties};
use log::info;
use rand::Rng;

const OBD_SERVICE_UUID: BleUuid = BleUuid::Uuid128([
    0x2c, 0x05, 0xad, 0xa8, 0x69, 0x58, 0x4d, 0x58, 0x84, 0x51, 0xf5, 0x8a, 0x11, 0x06, 0x88, 0x00,
]);

const OBD_COMMAND_CHARACTERISTIC_UUID: BleUuid = BleUuid::Uuid128([
    0x2c, 0x05, 0xad, 0xa8, 0x69, 0x58, 0x4d, 0x58, 0x84, 0x51, 0xf5, 0x8a, 0x11, 0x06, 0x88, 0x01,
]);

const OBD_RESPONSE_CHARACTERISTIC_UUID: BleUuid = BleUuid::Uuid128([
    0x2c, 0x05, 0xad, 0xa8, 0x69, 0x58, 0x4d, 0x58, 0x84, 0x51, 0xf5, 0x8a, 0x11, 0x06, 0x88, 0x02,
]);

pub fn init_obd_service(server: &mut BLEServer) {
    let obd_service = server.create_service(OBD_SERVICE_UUID);
    info!("Initialize OBD service");

    let command_characteristic = obd_service
        .lock()
        .create_characteristic(OBD_COMMAND_CHARACTERISTIC_UUID, NimbleProperties::WRITE);

    let response_characteristic = obd_service
        .lock()
        .create_characteristic(OBD_RESPONSE_CHARACTERISTIC_UUID, NimbleProperties::READ);

    command_characteristic.lock().on_write(move |args| {
        let data = args.recv_data;

        const MODE_01: u8 = 0x01;
        if data.first() == Some(&MODE_01) {
            let mut response = Vec::new();

            if let Some(pid) = data.get(1) {
                response = generate_response(*pid, 4);
            }

            info!("Data: {:?}", response);
            response_characteristic.lock().set_value(&response);
            response_characteristic.lock().notify();
        }
    });
}

fn generate_response(pid: u8, num_bytes: u8) -> Vec<u8> {
    let mut response = vec![0x41, pid];
    append_random_data(&mut response, num_bytes);

    response
}

fn append_random_data(response: &mut Vec<u8>, num_bytes: u8) {
    let mut rng = rand::thread_rng();
    for _ in 0..num_bytes {
        let random_byte: u8 = rng.gen();
        response.push(random_byte);
    }
}
