import java.util.ArrayList;
import java.util.List;

//Java intrinsic lock (com synchronized)
//Java lock (ReentrantLock)
//Java semaphore
//Exemplo completo

public class IntrinsicLockExamplo {
  static String compartilhada;
  static Integer mutex = 0;

  public static void main(String args[]) {
    Integer numThreads = Integer.parseInt(args[0]);
    List<Thread> threads = new ArrayList<>();

    for (int i = 0; i < numThreads; i++) {
      Thread t = new Thread(() -> {
        synchronized (mutex) {
          compartilhada = Thread.currentThread().getName();
          try {
            Thread.sleep(1000L);
          } catch (InterruptedException e) {
            // TODO Auto-generated catch block
          }

          System.out.println(compartilhada);
        }
      });
      threads.add(t);
    }

    threads.forEach(t -> t.start());
    threads.forEach(t -> {
      try {
        t.join();
      } catch (InterruptedException e) {
        // TODO Auto-generated catch block
      }
    });
  }
}
