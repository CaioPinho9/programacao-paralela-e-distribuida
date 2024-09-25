public class WaitNotify {
  public static void main(String[] args) {
    final Controlador controlador = new Controlador();

    Thread alo = new Thread(() -> {
      try {
        Thread.sleep(3000L);
      } catch (InterruptedException e) {
        // TODO Auto-generated catch block
      }
      synchronized (controlador) {
        System.out.print("Alô ");
        controlador.pronto = true;
        controlador.notify();
        // controlador.notifyAll();
      }

    });

    Runnable runnableMundo = () -> {
      synchronized (controlador) {
        while (!controlador.pronto) {
          try {
            System.out.println(Thread.currentThread().getName() //
                + " Esperando no while");
            controlador.wait();
          } catch (InterruptedException e) {
            // TODO Auto-generated catch block
          }
        }
        System.out.println("mundo!");
      }
    };
    Thread mundo = new Thread(runnableMundo);
    // Thread mundo1 = new Thread(runnableMundo);

    alo.start();
    mundo.start();
    // mundo1.start();

    try {
      mundo.join();
      alo.join();
      // mundo1.join();
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
    }
  }

  static class Controlador {
    public boolean pronto = false;
  }
}