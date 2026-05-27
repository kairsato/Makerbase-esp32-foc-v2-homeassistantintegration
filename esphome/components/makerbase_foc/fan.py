import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import CONF_ID
from . import MakerbaseFOC, makerbase_foc_ns

MakerbaseFOCFan = makerbase_foc_ns.class_("MakerbaseFOCFan", fan.Fan, cg.Component)

CONF_MOTOR_ID = "motor_id"

CONFIG_SCHEMA = fan.fan_schema(MakerbaseFOCFan).extend({
    cv.GenerateID(): cv.declare_id(MakerbaseFOCFan),
    cv.Required(CONF_MOTOR_ID): cv.use_id(MakerbaseFOC),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    parent = await cg.get_variable(config[CONF_MOTOR_ID])
    cg.add(var.set_parent(parent))