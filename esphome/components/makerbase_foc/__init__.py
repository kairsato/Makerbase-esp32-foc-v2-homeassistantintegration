import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import CONF_ID

DEPENDENCIES = []

makerbase_foc_ns = cg.esphome_ns.namespace("makerbase_foc")
MakerbaseFOCFan = makerbase_foc_ns.class_("MakerbaseFOCFan", fan.Fan, cg.Component)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(MakerbaseFOCFan),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
