import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["fan"]

makerbase_foc_ns = cg.esphome_ns.namespace("makerbase_foc")
MakerbaseFOC = makerbase_foc_ns.class_("MakerbaseFOC", cg.Component, uart.UARTDevice)

CONF_MAKERBASE_FOC_ID = "makerbase_foc_id"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MakerbaseFOC),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)