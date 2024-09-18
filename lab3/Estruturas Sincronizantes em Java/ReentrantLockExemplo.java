import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

public class ReentrantLockExemplo {
  static String compartilhada;
  static ReentrantLock lock = new ReentrantLock();

  public static void main(String args[]) {
    Integer numThreads = Integer.parseInt(args[0]);
    List<Thread> threads = new ArrayList<>();

    for (int i = 0; i < numThreads; i++) {
      Thread t = new Thread(() -> {
        try {
          lock.lock();
          compartilhada = Thread.currentThread().getName();
          try {
            Thread.sleep(1000L);
          } catch (InterruptedException e) {
            // TODO Auto-generated catch block
          }

          System.out.println(compartilhada);
        } finally {
          lock.unlock();
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
