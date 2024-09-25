import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class Barreiras {
  static CyclicBarrier barreira = new CyclicBarrier(3);
  public static void main(String[] args) {
    Thread alo = new Thread(metodoAlo());
    Thread mundo = new Thread(metodoMundo());
    Thread mundo1 = new Thread(metodoMundo());

    alo.start();
    mundo.start();
    mundo1.start();

    try {
      mundo.join();
      alo.join();
      mundo1.join();
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
    }
  }

  private static Runnable metodoMundo() {
    return () -> {
      try {
        barreira.await();
      } catch (InterruptedException | BrokenBarrierException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
      System.out.println("mundo!");
    };
  }

  private static Runnable metodoAlo() {
    return () -> {
      try {
        Thread.sleep(2000L);
      } catch (InterruptedException e) {
        // TODO Auto-generated catch block
      }
      System.out.print("Alô ");
      try {
        barreira.await();
      } catch (InterruptedException | BrokenBarrierException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
    };
  }
}