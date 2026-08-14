#pragma once

#include <mbgl/style/conversion.hpp>
#include <mbgl/style/conversion_impl.hpp>
#include <mbgl/style/layer.hpp>
#include <mbgl/style/source.hpp>
#include <mbgl/util/geojson.hpp>
#include <mbgl/style/property_value.hpp>

#include <cmath>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "rust/cxx.h"

namespace mln::bridge {

// Primary template intentionally undefined — forces explicit specialization
template <typename T>
struct PropertyTypeTag;

enum class PropertyType : uint8_t {
    Undefined,
    Bool,
    Int,
    Float,
    String,
    RasterResamplingType,
    SymbolAnchorType,

};

#define DEFINE_PROPERTY_TYPE_TAG(CppT, EnumVal)                       \
    template <> struct PropertyTypeTag<CppT> {                        \
        static constexpr PropertyType value = PropertyType::EnumVal;  \
    };                                                                \

#define DEFINE_PROPERTY_SHIMS(Name, CppT, EnumVal)                        \
    mln::bridge::PropertyType property_value_##Name##_type(const PropertyValue##Name&) { \
        return mln::bridge::PropertyType::EnumVal;                                     \
    }                                                                     \
    CppT property_value_##Name##_get(const PropertyValue##Name& pv) {     \
        return pv.value.asConstant();                                     \
    }

DEFINE_PROPERTY_TYPE_TAG(bool, Bool);
DEFINE_PROPERTY_TYPE_TAG(int32_t, Int);
DEFINE_PROPERTY_TYPE_TAG(float, Float);
DEFINE_PROPERTY_TYPE_TAG(std::string, String);

class PropertyValueUndefined {
    mbgl::style::PropertyValue<mbgl::style::Undefined> value;
};

struct PropertyValueInt {
private:
    mbgl::style::PropertyValue<int32_t> value;
public:
    DEFINE_PROPERTY_SHIMS(Int, int32_t, Int)
};

struct PropertyValueDouble {
private:
    mbgl::style::PropertyValue<double> value;
public:
    DEFINE_PROPERTY_SHIMS(Double, double, Double)
};

struct PropertyValueString {
private:
    mbgl::style::PropertyValue<std::string> value;
public:
    DEFINE_PROPERTY_SHIMS(String, std::string, String)
};
