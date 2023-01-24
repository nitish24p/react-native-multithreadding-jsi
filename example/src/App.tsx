import * as React from 'react';

import { StyleSheet, View, Text, Button, TextInput } from 'react-native';
import {
  multiply,
  spawnTask,
  spawnTaskAsync,
} from 'react-native-multithreadding-lite';

function fibonacci(n) {
  if (n <= 2) {
    return 1;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}
export default function App() {
  const [result, setResult] = React.useState<number | undefined>();
  const [ticker, setTicker] = React.useState<number | undefined>(0);
  const [text, onChangeText] = React.useState('Useless Text');

  React.useEffect(() => {
    const interval = setInterval(() => {
      setTicker((t) => t + 1);
    }, 200);

    return () => clearInterval(interval);
  }, []);

  React.useEffect(() => {
    multiply(3, 7).then(setResult);
  }, []);

  const someCallBackWhichIsAsync = () => {
    console.log(' This is in same thread');
    spawnTask(() => {
      setResult(fibonacci(40));
    });

    //setResult(fibonacci(40));
    Promise.resolve(() => {
      setResult(fibonacci(30));
    });
    console.log('This was done');
  };

  const globalPromise = async () => {
    console.log(' This is in same thread');
    // spawnTask(() => {

    // });
    try {
      spawnTaskAsync(() => fibonacci(40)).then(console.log);
      //setResult(result);
    } catch (error) {
      console.error(error);
    }

    //setResult(fibonacci(40));
    console.log('This was done');
  };

  return (
    <View style={styles.container}>
      <Text>Result: {result}</Text>
      <Text>Ticker: {ticker}</Text>
      <TextInput
        style={styles.input}
        onChangeText={onChangeText}
        value={text}
      />
      <Button title="Press Me" onPress={someCallBackWhichIsAsync} />
      <Button title="Press Me Async" onPress={globalPromise} />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
  input: {
    height: 40,
    margin: 12,
    borderWidth: 1,
    padding: 10,
  },
});
