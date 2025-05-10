var __defProp = Object.defineProperty;
var __defNormalProp = (obj, key, value) => key in obj ? __defProp(obj, key, { enumerable: true, configurable: true, writable: true, value }) : obj[key] = value;
var __publicField = (obj, key, value) => {
  __defNormalProp(obj, typeof key !== "symbol" ? key + "" : key, value);
  return value;
};

// src/utilities.ts
function NormalizeClampVector(x, y, max) {
  const length = Math.sqrt(x * x + y * y);
  if (length > max)
    return { x: x / length, y: y / length };
  else
    return { x: x / max, y: y / max };
}

// src/GamepadEmulator.ts
var DEFAULT_GPAD_BUTTON_COUNT = 18;
var DEFAULT_GPAD_AXIS_COUNT = 4;
var _GamepadEmulator = class {
  getNativeGamepads = () => [];
  buttonPressThreshold = 0.1;
  realGpadToPatchedIndexMap = [];
  patchedGpadToRealIndexMap = [];
  emulatedGamepads = [];
  emulatedGamepadsMetadata = [];
  undoEventPatch = () => {
  };
  constructor(buttonPressThreshold) {
    this.buttonPressThreshold = buttonPressThreshold || this.buttonPressThreshold;
    if (_GamepadEmulator.instanceRunning)
      throw new Error("Only one GamepadEmulator instance may exist at a time!");
    _GamepadEmulator.instanceRunning = true;
    this.undoEventPatch = this.monkeyPatchGamepadEvents();
    this.monkeyPatchGetGamepads();
  }
  AddEmulatedGamepad(gpadIndex, overlayMode, buttonCount = DEFAULT_GPAD_BUTTON_COUNT, axisCount = DEFAULT_GPAD_AXIS_COUNT) {
    if (gpadIndex === -1 || !gpadIndex && gpadIndex !== 0)
      gpadIndex = this.nextEmptyEGpadIndex(overlayMode);
    if (this.emulatedGamepads[gpadIndex])
      return false;
    const eGpad = {
      emulation: "emulated" /* emulated */,
      connected: true,
      timestamp: performance.now(),
      displayId: "Emulated Gamepad " + gpadIndex,
      id: "Emulated Gamepad " + gpadIndex + " (Xinput STANDARD GAMEPAD)",
      mapping: "standard",
      index: gpadIndex,
      buttons: new Array(buttonCount).fill({ pressed: false, value: 0, touched: false }, 0, buttonCount),
      axes: new Array(axisCount).fill(0, 0, axisCount),
      hapticActuators: []
    };
    this.emulatedGamepads[gpadIndex] = eGpad;
    this.emulatedGamepadsMetadata[gpadIndex] = { overlayMode };
    const event = new Event("gamepadconnected");
    event.gamepad = eGpad;
    window.dispatchEvent(event);
    return eGpad;
  }
  RemoveEmulatedGamepad(gpadIndex) {
    this.ClearDisplayButtonEventListeners(gpadIndex);
    this.ClearDisplayJoystickEventListeners(gpadIndex);
    var e_gpad = this.emulatedGamepads[gpadIndex];
    if (e_gpad) {
      delete this.emulatedGamepads[gpadIndex];
      delete this.emulatedGamepadsMetadata[gpadIndex];
      const gpad = {
        ...e_gpad,
        connected: false,
        timestamp: performance.now()
      };
      const event = new Event("gamepaddisconnected");
      event.gamepad = gpad;
      window.dispatchEvent(event);
    } else {
      console.warn("Error: removing emulated gamepad. No emulated gamepad exists at index " + gpadIndex);
    }
  }
  PressButton(gpadIndex, buttonIndex, value, touched) {
    if (this.emulatedGamepads[gpadIndex] === void 0)
      throw new Error("Error: PressButton() - no emulated gamepad at index " + gpadIndex + ", pass a valid index, or call AddEmulatedGamepad() first to create an emulated gamepad at that index");
    var isPressed = value > this.buttonPressThreshold;
    this.emulatedGamepads[gpadIndex].buttons[buttonIndex] = {
      pressed: isPressed,
      value: value || 0,
      touched: isPressed || touched || false
    };
  }
  MoveAxis(gpadIndex, axisIndex, value) {
    if (!this.emulatedGamepads[gpadIndex])
      throw new Error("Error: MoveAxis() - no emulated gamepad at index " + gpadIndex + ", pass a valid index, or call AddEmulatedGamepad() first to create an emulated gamepad at that index");
    this.emulatedGamepads[gpadIndex].axes[axisIndex] = value;
  }
  AddDisplayButtonEventListeners(gpadIndex, buttonConfigs) {
    if (!this.emulatedGamepads[gpadIndex])
      throw new Error("Error: AddDisplayJoystickEventListeners() - no emulated gamepad at index " + gpadIndex + ", pass a valid index, or call AddEmulatedGamepad() first to create an emulated gamepad at that index");
    let removeListenerFuncs = [];
    for (var i = 0; i < buttonConfigs.length; i++) {
      const btnConfig = buttonConfigs[i];
      const gpadButtonIndex = btnConfig.buttonIndex;
      const tapTarget = btnConfig.tapTarget;
      if (!tapTarget) {
        console.warn("AddDisplayButtonEventListeners() - no tap target in button config " + gpadButtonIndex + ", skipping...");
        continue;
      }
      const touchStartHandler = (event) => {
        if (event.changedTouches[0].target == tapTarget)
          event.preventDefault();
      };
      window.addEventListener("touchstart", touchStartHandler, { passive: false });
      const pointerEnterHandler = (e) => {
        const pressAmt = e.buttons == 1 ? 1 : 0;
        if (!btnConfig.lockTargetWhilePressed || pressAmt == 0)
          this.PressButton(gpadIndex, gpadButtonIndex, pressAmt, true);
      };
      tapTarget.addEventListener("pointerenter", pointerEnterHandler);
      const pointerExitHandler = (e) => {
        const pressAmt = e.buttons == 1 ? 1 : 0;
        if (!btnConfig.lockTargetWhilePressed || pressAmt == 0)
          this.PressButton(gpadIndex, gpadButtonIndex, 0, false);
      };
      tapTarget.addEventListener("pointerleave", pointerExitHandler);
      const pointerCancelHandler = (e) => {
      };
      tapTarget.addEventListener("pointercancel", pointerCancelHandler);
      if (btnConfig.type == "onOff" /* onOff */) {
        const pointerDownHandler = (e) => {
          e.preventDefault();
          this.PressButton(gpadIndex, gpadButtonIndex, 1, true);
          if (btnConfig.lockTargetWhilePressed)
            tapTarget.setPointerCapture(e.pointerId);
          else
            tapTarget.releasePointerCapture(e.pointerId);
          console.log("on/off pointer down", tapTarget.hasPointerCapture(e.pointerId));
        };
        tapTarget.addEventListener("pointerdown", pointerDownHandler);
        const pointerUpHandler = () => {
          this.PressButton(gpadIndex, gpadButtonIndex, 0, true);
        };
        tapTarget.addEventListener("pointerup", pointerUpHandler);
        removeListenerFuncs.push(function removeListeners() {
          window.removeEventListener("touchstart", touchStartHandler);
          tapTarget.removeEventListener("pointerenter", pointerEnterHandler);
          tapTarget.removeEventListener("pointerleave", pointerExitHandler);
          tapTarget.removeEventListener("pointerdown", pointerDownHandler);
          tapTarget.removeEventListener("pointerup", pointerUpHandler);
          tapTarget.removeEventListener("pointercancel", pointerCancelHandler);
        });
      } else if (btnConfig.type == "variable" /* variable */) {
        const config = { ...btnConfig };
        const removeDragListeners = this.AddDragControlListener(config, (pointerDown, xValue, yValue) => {
          let value = pointerDown ? this.buttonPressThreshold + 1e-5 : 0;
          value += btnConfig.directions["left" /* left */] || btnConfig.directions["right" /* right */] ? Math.abs(xValue) : 0;
          value += btnConfig.directions["up" /* up */] || btnConfig.directions["down" /* down */] ? Math.abs(yValue) : 0;
          this.PressButton(gpadIndex, btnConfig.buttonIndex, Math.min(value, 1), pointerDown);
        });
        removeListenerFuncs.push(function removeListeners() {
          window.removeEventListener("touchstart", touchStartHandler);
          tapTarget.removeEventListener("pointerenter", pointerEnterHandler);
          tapTarget.removeEventListener("pointerleave", pointerExitHandler);
          tapTarget.removeEventListener("pointercancel", pointerCancelHandler);
          removeDragListeners();
        });
      }
    }
    ;
    this.emulatedGamepadsMetadata[gpadIndex].removeButtonListenersFunc = () => {
      removeListenerFuncs.forEach((func) => func());
    };
  }
  AddDisplayJoystickEventListeners(gpadIndex, joystickConfigs) {
    if (!this.emulatedGamepads[gpadIndex])
      throw new Error("Error: AddDisplayJoystickEventListeners() - no emulated gamepad at index " + gpadIndex + ", pass a valid index, or call AddEmulatedGamepad() first to create an emulated gamepad at that index");
    let removeListenerFuncs = [];
    for (let i = 0; i < joystickConfigs.length; i++) {
      const config = joystickConfigs[i];
      if (config.tapTarget == void 0) {
        console.warn(`AddDisplayJoystickEventListeners() - no tap target in joystick config ${i}, skipping...`);
        continue;
      }
      const removeDragListeners = this.AddDragControlListener(config, (_, xValue, yValue) => {
        if (config.xAxisIndex !== void 0)
          this.MoveAxis(gpadIndex, config.xAxisIndex, xValue);
        if (config.yAxisIndex !== void 0)
          this.MoveAxis(gpadIndex, config.yAxisIndex, yValue);
      });
      removeListenerFuncs.push(removeDragListeners);
    }
    this.emulatedGamepadsMetadata[gpadIndex].removeJoystickListenersFunc = () => {
      removeListenerFuncs.forEach((func) => func());
    };
  }
  ClearDisplayButtonEventListeners(gpadIndex) {
    if (this.emulatedGamepadsMetadata[gpadIndex] && this.emulatedGamepadsMetadata[gpadIndex]?.removeButtonListenersFunc)
      this.emulatedGamepadsMetadata[gpadIndex].removeButtonListenersFunc();
  }
  ClearDisplayJoystickEventListeners(gpadIndex) {
    if (this.emulatedGamepadsMetadata[gpadIndex] && this.emulatedGamepadsMetadata[gpadIndex]?.removeJoystickListenersFunc)
      this.emulatedGamepadsMetadata[gpadIndex].removeJoystickListenersFunc();
  }
  AddDragControlListener(config, callback) {
    let touchDetails = {
      startX: 0,
      startY: 0
    };
    let activePointerId = -1;
    const pointerMoveHandler = (moveEvent) => {
      var pointerId = moveEvent.pointerId;
      if (activePointerId === pointerId) {
        const xMin = config.directions["left" /* left */] ? -1 : 0;
        const xMax = config.directions["right" /* right */] ? 1 : 0;
        const yMin = config.directions["up" /* up */] ? -1 : 0;
        const yMax = config.directions["down" /* down */] ? 1 : 0;
        const deltaX = moveEvent.pageX - touchDetails.startX;
        const deltaY = moveEvent.pageY - touchDetails.startY;
        let { x, y } = NormalizeClampVector(deltaX, deltaY, config.dragDistance);
        x = Math.max(Math.min(x, xMax), xMin);
        y = Math.max(Math.min(y, yMax), yMin);
        if (config.invertX)
          x = -x;
        if (config.invertY)
          y = -y;
        if (config.swapAxes) {
          callback(true, y, x);
        } else {
          callback(true, x, y);
        }
      }
    };
    const pointerUpHandler = (upEvent) => {
      if (activePointerId == upEvent.pointerId) {
        document.removeEventListener("pointermove", pointerMoveHandler, false);
        document.removeEventListener("pointerup", pointerUpHandler, false);
        activePointerId = -1;
        callback(false, 0, 0);
      }
    };
    config.tapTarget.addEventListener("pointerdown", (downEvent) => {
      downEvent.preventDefault();
      touchDetails.startX = downEvent.pageX;
      touchDetails.startY = downEvent.pageY;
      activePointerId = downEvent.pointerId;
      if (config.lockTargetWhilePressed)
        config.tapTarget.setPointerCapture(downEvent.pointerId);
      else
        config.tapTarget.releasePointerCapture(downEvent.pointerId);
      callback(true, 0, 0);
      document.addEventListener("pointermove", pointerMoveHandler, false);
      document.addEventListener("pointerup", pointerUpHandler, false);
    });
    const touchStartHandler = (event) => {
      if (event.changedTouches[0].target == config.tapTarget) {
        event.preventDefault();
      }
    };
    window.addEventListener("touchstart", touchStartHandler, { passive: false });
    return function removeListeners() {
      window.removeEventListener("touchstart", touchStartHandler);
      config.tapTarget.removeEventListener("pointerdown", pointerMoveHandler);
    };
  }
  cloneGamepad(original) {
    if (!original)
      return original;
    const axesCount = original.axes ? original.axes.length : 0;
    const buttonsCount = original.buttons ? original.buttons.length : 0;
    const clone = {};
    for (let key in original) {
      if (key === "axes") {
        const axes = new Array(axesCount);
        for (let i = 0; i < axesCount; i++) {
          axes[i] = Number(original.axes[i]);
        }
        Object.defineProperty(clone, "axes", { value: axes, enumerable: true });
      } else if (key === "buttons") {
        const buttons = new Array(buttonsCount);
        for (let i = 0; i < buttonsCount; i++) {
          const btn = original.buttons[i];
          if (btn == void 0)
            buttons[i] = btn;
          else {
            const pressed = btn.pressed, value = btn.value, touched = btn.touched || false;
            buttons[i] = { pressed, value, touched };
          }
        }
        Object.defineProperty(clone, "buttons", { value: buttons, enumerable: true });
      } else {
        Object.defineProperty(clone, key, { get: () => {
          return original[key];
        }, configurable: true, enumerable: true });
      }
    }
    if (!clone.emulation)
      clone.emulation = "real" /* real */;
    return clone;
  }
  nextEmptyEGpadIndex(overlayMode) {
    let index = 0;
    if (overlayMode) {
      do {
        if (!this.emulatedGamepads[index])
          break;
        index++;
      } while (index < this.emulatedGamepads.length);
    } else {
      const end = Math.max(this.emulatedGamepads.length, this.patchedGpadToRealIndexMap.length);
      do {
        if (!this.emulatedGamepads[index] && this.patchedGpadToRealIndexMap[index] == void 0)
          break;
        console.log("nextEmptyEGpadIndex", index, !this.emulatedGamepads[index], this.patchedGpadToRealIndexMap[index] == void 0);
        index++;
      } while (index < end);
    }
    return index;
  }
  nextEmptyRealGpadIndex(startingIndex) {
    let index = startingIndex;
    const end = Math.max(this.emulatedGamepads.length, this.patchedGpadToRealIndexMap.length);
    do {
      const emulatedGpadMetadata = this.emulatedGamepadsMetadata[index];
      const realGpadEmptySpot = this.realGpadToPatchedIndexMap[index] == void 0 && this.patchedGpadToRealIndexMap[index] == void 0;
      if (!!emulatedGpadMetadata && emulatedGpadMetadata.overlayMode || !emulatedGpadMetadata && realGpadEmptySpot)
        break;
      index++;
    } while (index < end);
    return index;
  }
  monkeyPatchGamepadEvents() {
    let onGamepadConnectedProps, onGamepadDisconnectedProps, windowOngamepadconnected, windowOngamepaddisconnected;
    if (window.hasOwnProperty("ongamepadconnected")) {
      onGamepadConnectedProps = Object.getOwnPropertyDescriptor(window, "ongamepadconnected");
      windowOngamepadconnected = window.ongamepadconnected;
      window.ongamepadconnected = null;
      Object.defineProperty(window, "ongamepadconnected", {
        get: () => function(ev) {
        },
        set: (fn) => {
          windowOngamepadconnected = fn;
        },
        configurable: true
      });
    }
    if (window.hasOwnProperty("ongamepaddisconnected")) {
      onGamepadDisconnectedProps = Object.getOwnPropertyDescriptor(window, "ongamepaddisconnected");
      windowOngamepaddisconnected = window.ongamepaddisconnected;
      window.ongamepaddisconnected = null;
      Object.defineProperty(window, "ongamepaddisconnected", {
        get: () => function(ev) {
        },
        set: (fn) => {
          windowOngamepadconnected = fn;
        },
        configurable: true
      });
    }
    const gamepadConnectedHandler = (e) => {
      const gpad = e.gamepad;
      if (gpad && gpad.emulation === void 0) {
        e.stopImmediatePropagation();
        const eGpad = this.cloneGamepad(e.gamepad);
        const gpadIndex = eGpad.index;
        const mappedIndex = this.nextEmptyRealGpadIndex(gpadIndex);
        this.realGpadToPatchedIndexMap[gpadIndex] = mappedIndex;
        this.patchedGpadToRealIndexMap[mappedIndex] = gpadIndex;
        Object.defineProperty(eGpad, "index", { get: () => mappedIndex });
        Object.defineProperty(eGpad, "emulation", { get: () => "real" /* real */ });
        console.log(`real gamepad connected ${eGpad.id} (${gpadIndex}>${mappedIndex})`, this.realGpadToPatchedIndexMap, this.emulatedGamepads, this.emulatedGamepadsMetadata);
        e = new Event("gamepadconnected");
        e.gamepad = eGpad;
        window.dispatchEvent(e);
      }
      console.log("windowOngamepadconnected", windowOngamepadconnected);
      if (windowOngamepadconnected)
        windowOngamepadconnected.call(window, e);
    };
    window.addEventListener("gamepadconnected", gamepadConnectedHandler);
    const gamepadDisconnectedHandler = (e) => {
      const raw_gpad = e.gamepad;
      if (raw_gpad && raw_gpad.emulation === void 0) {
        e.stopImmediatePropagation();
        const clone = this.cloneGamepad(e.gamepad);
        const mappedIndex = this.realGpadToPatchedIndexMap[clone.index] || clone.index;
        Object.defineProperty(clone, "index", { get: () => mappedIndex });
        delete this.realGpadToPatchedIndexMap[clone.index];
        delete this.patchedGpadToRealIndexMap[mappedIndex];
        e = new Event("gamepaddisconnected");
        e.gamepad = clone;
        window.dispatchEvent(e);
      }
      if (windowOngamepaddisconnected)
        windowOngamepaddisconnected.call(window, e);
    };
    window.addEventListener("gamepaddisconnected", gamepadDisconnectedHandler);
    return function cleanup() {
      window.removeEventListener("gamepadconnected", gamepadConnectedHandler);
      if (window.hasOwnProperty("ongamepadconnected")) {
        Object.defineProperty(window, "ongamepadconnected", onGamepadConnectedProps);
        window.ongamepadconnected = windowOngamepadconnected;
      }
      window.removeEventListener("gamepaddisconnected", gamepadDisconnectedHandler);
      if (window.hasOwnProperty("ongamepaddisconnected")) {
        Object.defineProperty(window, "ongamepaddisconnected", onGamepadDisconnectedProps);
        window.ongamepaddisconnected = windowOngamepaddisconnected;
      }
    };
  }
  monkeyPatchGetGamepads() {
    const self = this;
    let getNativeGamepads = navigator.getGamepads || navigator.webkitGetGamepads || navigator.mozGetGamepads || navigator.msGetGamepads;
    if (getNativeGamepads)
      this.getNativeGamepads = getNativeGamepads;
    navigator.getGamepads = function() {
      let nativeGpads = [];
      let nativeGpadsRaw = getNativeGamepads != void 0 ? self.getNativeGamepads.apply(navigator) : [];
      for (let i = 0; i < nativeGpadsRaw.length; i++) {
        const gpad = nativeGpadsRaw[i];
        if (!gpad)
          continue;
        let clone = self.cloneGamepad(gpad);
        let mappedIndex = self.realGpadToPatchedIndexMap[clone.index] || clone.index;
        Object.defineProperty(clone, "index", { get: () => mappedIndex });
        nativeGpads[mappedIndex] = clone;
      }
      let emulatedGpads = self.emulatedGamepads;
      for (let i = 0; i < emulatedGpads.length; i++) {
        let n_gpad = nativeGpads[i];
        let e_gpad = emulatedGpads[i];
        if (e_gpad && n_gpad) {
          n_gpad.emulation = "overlay" /* overlay */;
          let btnCount = Math.max(n_gpad.buttons.length, e_gpad.buttons.length);
          for (let btnIdx = 0; btnIdx < btnCount; btnIdx++) {
            const e_btn = e_gpad.buttons[btnIdx] || { touched: false, pressed: false, value: 0 };
            const n_btn = n_gpad.buttons[btnIdx] || { touched: false, pressed: false, value: 0 };
            nativeGpads[i].buttons[btnIdx] = {
              touched: e_btn.touched || n_btn.touched || false,
              pressed: e_btn.pressed || n_btn.pressed || false,
              value: Math.max(e_btn.value, n_btn.value) || 0
            };
          }
          let axisCount = Math.max(e_gpad.axes.length, n_gpad.axes.length);
          for (let axisIndex = 0; axisIndex < axisCount; axisIndex++) {
            const e_axis = e_gpad.axes[axisIndex] || 0;
            const n_axis = n_gpad.axes[axisIndex] || 0;
            nativeGpads[i].axes[axisIndex] = Math.abs(e_axis || 0) > Math.abs(n_axis || 0) ? e_axis || 0 : n_axis || 0;
          }
        } else if (e_gpad) {
          e_gpad.emulation = "emulated" /* emulated */;
          e_gpad.timestamp = performance.now();
          nativeGpads[i] = self.cloneGamepad(e_gpad);
        }
      }
      return nativeGpads;
    };
  }
  cleanup() {
    for (let i = 0; i < this.emulatedGamepads.length; i++) {
      this.ClearDisplayButtonEventListeners(i);
      this.ClearDisplayJoystickEventListeners(i);
    }
    this.emulatedGamepads = [];
    this.undoEventPatch();
    navigator.getGamepads = this.getNativeGamepads;
    _GamepadEmulator.instanceRunning = false;
  }
};
var GamepadEmulator = _GamepadEmulator;
__publicField(GamepadEmulator, "instanceRunning", false);
export {
  DEFAULT_GPAD_AXIS_COUNT,
  DEFAULT_GPAD_BUTTON_COUNT,
  GamepadEmulator
};
