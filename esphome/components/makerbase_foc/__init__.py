import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = []
AUTO_LOAD = ["fan"]

makerbase_foc_ns = cg.esphome_ns.namespace("makerbase_foc")
MakerbaseFOC = makerbase_foc_ns.class_("MakerbaseFOC", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MakerbaseFOC),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    cg.add_library("Simple FOC", "2.3.3")
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)