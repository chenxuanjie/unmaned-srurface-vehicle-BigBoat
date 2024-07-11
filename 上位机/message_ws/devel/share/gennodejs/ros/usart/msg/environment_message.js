// Auto-generated. Do not edit!

// (in-package usart.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class environment_message {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.voltage_left_battery = null;
      this.voltage_right_battery = null;
      this.temperature = null;
    }
    else {
      if (initObj.hasOwnProperty('voltage_left_battery')) {
        this.voltage_left_battery = initObj.voltage_left_battery
      }
      else {
        this.voltage_left_battery = 0.0;
      }
      if (initObj.hasOwnProperty('voltage_right_battery')) {
        this.voltage_right_battery = initObj.voltage_right_battery
      }
      else {
        this.voltage_right_battery = 0.0;
      }
      if (initObj.hasOwnProperty('temperature')) {
        this.temperature = initObj.temperature
      }
      else {
        this.temperature = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type environment_message
    // Serialize message field [voltage_left_battery]
    bufferOffset = _serializer.float64(obj.voltage_left_battery, buffer, bufferOffset);
    // Serialize message field [voltage_right_battery]
    bufferOffset = _serializer.float64(obj.voltage_right_battery, buffer, bufferOffset);
    // Serialize message field [temperature]
    bufferOffset = _serializer.float64(obj.temperature, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type environment_message
    let len;
    let data = new environment_message(null);
    // Deserialize message field [voltage_left_battery]
    data.voltage_left_battery = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [voltage_right_battery]
    data.voltage_right_battery = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [temperature]
    data.temperature = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'usart/environment_message';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4be2f61eda002ec29f7656590853bb61';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 voltage_left_battery
    float64 voltage_right_battery
    float64 temperature
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new environment_message(null);
    if (msg.voltage_left_battery !== undefined) {
      resolved.voltage_left_battery = msg.voltage_left_battery;
    }
    else {
      resolved.voltage_left_battery = 0.0
    }

    if (msg.voltage_right_battery !== undefined) {
      resolved.voltage_right_battery = msg.voltage_right_battery;
    }
    else {
      resolved.voltage_right_battery = 0.0
    }

    if (msg.temperature !== undefined) {
      resolved.temperature = msg.temperature;
    }
    else {
      resolved.temperature = 0.0
    }

    return resolved;
    }
};

module.exports = environment_message;
