use esp32_nimble::{utilities::BleUuid, BLEDevice};
use esp_idf_sys as _;
use log::{info, warn};

mod obd_service;

const SERVICE_UUID: BleUuid = BleUuid::Uuid128([
    0x2c, 0x05, 0xad, 0xa8, 0x69, 0x58, 0x4d, 0x58, 0x84, 0x51, 0xf5, 0x8a, 0x11, 0x06, 0x87, 0x9e,
]);

fn main() {
    esp_idf_sys::link_patches();
    esp_idf_svc::log::EspLogger::initialize_default();

    let ble_device = BLEDevice::take();

    let mut server = ble_device.get_server();

    obd_service::init_obd_service(&mut server);

    server.on_connect(|server, desc| {
        ::log::info!("Client connected to BLE server");

        let params = (desc.conn_handle, 24, 48, 0, 60);
        match server.update_conn_params(params.0, params.1, params.2, params.3, params.4) {
            Ok(_) => info!("Connection parameters updated"),
            Err(err) => warn!("Connection parameters update failed\nError: {:?}", err),
        }

        match ble_device.get_advertising().start() {
            Ok(_) => info!("Advertising started"),
            Err(err) => warn!("Failed to start advertising\nError: {:?}", err),
        }
    });

    let _service = server.create_service(SERVICE_UUID);

    let ble_advertising = ble_device.get_advertising();
    ble_advertising
        .name("ESP32-OBD-SIM")
        .add_service_uuid(SERVICE_UUID);

    match ble_advertising.start() {
        Ok(_) => info!("Advertising started"),
        Err(err) => warn!("Failed to start advertising\nError: {:?}", err),
    }

    loop {
        esp_idf_hal::delay::FreeRtos::delay_ms(1000);
    }
}
