import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class Completo {
  static ReentrantLock lock;
  static Semaphore semaphore;
  static int contadorDePermissoes;

  public static void main(String args[]) {
    Integer numThreads = Integer.parseInt(args[0]);
    List<Thread> threads = new ArrayList<>();
    semaphore = new Semaphore(2);
    lock = new ReentrantLock();

    for (int i = 0; i < numThreads; i++) {
      Thread t = new Thread(() -> {
        try {
          System.out.println(Thread.currentThread().getName() + " Está esperando para entrar na região crítica");
          semaphore.acquire();
          lock.lock();
          contadorDePermissoes++;
          System.out.println(Thread.currentThread().getName() + " Entrou na região crítica com contador de permissões: "
              + contadorDePermissoes);
          lock.unlock();

          Thread.sleep(1000L);
          semaphore.release();
          lock.lock();
          contadorDePermissoes--;
          System.out.println(Thread.currentThread().getName() + " Saiu na região crítica com contador de permissões: "
              + contadorDePermissoes);
          lock.unlock();
        } catch (InterruptedException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
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
