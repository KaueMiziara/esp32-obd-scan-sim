use esp_idf_sys as _;
use esp32_nimble::{uuid128, BLEDevice};
use log::{info, warn};

fn main() {
    esp_idf_sys::link_patches();
    esp_idf_svc::log::EspLogger::initialize_default();

    let ble_device = BLEDevice::take();

    let server = ble_device.get_server();
    server.on_connect(|server, desc| {
        ::log::info!("Client conencted");

        let params = (desc.conn_handle, 24, 48, 0, 60);
        match server.update_conn_params(
            params.0,
            params.1, 
            params.2, 
            params.3, 
            params.4,
        ) {
            Ok(_) => info!("Connection parameters updated"),
            Err(err) => warn!(
                "Connection parameters update failed\nError: {:?}", err
            ),
        }

        match ble_device.get_advertising().start() {
            Ok(_) => info!("Start advertising"),
            Err(err) => warn!(
                "Failed to start advertising\nError: {:?}", err
            ),
        }
    });

    let _service = server.create_service(uuid128!("2c05ada8-6958-4d58-8451-f58a1106879e"));

    let ble_advertising = ble_device.get_advertising();
    ble_advertising
        .name("ESP32-OBD-SIM")
        .add_service_uuid(uuid128!("2c05ada8-6958-4d58-8451-f58a1106879e"));

    match ble_advertising.start() {
        Ok(_) => info!("Start advertising"),
        Err(err) => warn!(
            "Failed to start advertising\nError: {:?}", err
        ),
    }

    loop {
        esp_idf_hal::delay::FreeRtos::delay_ms(1000);
    }
}
