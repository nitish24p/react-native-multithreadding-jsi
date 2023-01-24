import { NativeModules, Platform } from 'react-native';

declare global {
  namespace globalThis {
    var spawnTask: (callback: () => any) => void;
    var spawnTaskAsync: (cb: () => any) => Promise<any>;
  }
}

const LINKING_ERROR =
  `The package 'react-native-multithreadding-lite' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

const MultithreaddingLite = NativeModules.MultithreaddingLite
  ? NativeModules.MultithreaddingLite
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export function multiply(a: number, b: number): Promise<number> {
  return MultithreaddingLite.multiply(a, b);
}

export function spawnTask(cb: () => void) {
  global.spawnTask(cb);
}

export function spawnTaskAsync(cb: () => any): Promise<any> {
  return global.spawnTaskAsync(cb);
}
