import java.util.Queue;
import java.util.LinkedList;

class Buffer {
    private Queue<Integer> buffer = new LinkedList<>();
    private int max_size;
    
    Buffer(int size) {
        this.max_size = size;
    }
    
    public synchronized void put(int value, int id) {
        try { 
            while(buffer.size() == this.max_size){
                System.out.println("Produtor("+id+") bloqueado");
                wait();
            }
            buffer.add(value);
            System.out.println("Produtor("+id+") adicionando elemento");
            notifyAll();
        } catch (InterruptedException e) { }
    }
    
    public synchronized int get(int id) {
        try { 
            while(buffer.size() == 0){
                System.out.println("Consumidor("+id+") bloqueado");
                wait();
            }
            int value = buffer.remove();
            System.out.println("Consumidor("+id+") retirando elemento");
            notifyAll();
            return value;
        } catch (InterruptedException e) { return -1;}
    }
    
    public synchronized void print_buffer() {
        System.out.println(this.buffer.toString());
    }
}

class Producer extends Thread {
    int id;
    int delay;
    Buffer buffer;
    
    Producer(int id, int delayTime, Buffer buffer) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = buffer;
    }
    
    public void run() {
        try {
            for (;;) {
                this.buffer.put(1, this.id);
                sleep(this.delay); 
            }
        } catch (InterruptedException e) { return; }
    }
}

class Consumer extends Thread {
    int id;
    int delay;
    Buffer buffer;
    
    Consumer(int id, int delayTime, Buffer buffer) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = buffer;
    }
    
    public void run() {
        try {
            for (;;) {
                this.buffer.get(this.id);
                sleep(this.delay); 
            }
        } catch (InterruptedException e) { return; }
    }
}



public class Main{
    static final int num_producers = 2;
    static final int num_consumers = 1;
    static final int buffer_size = 10;
    
	public static void main(String[] args) {
	    System.out.println("Começando o programa!");
	    
		int i;
		Buffer buffer = new Buffer(buffer_size);
		Producer[] producers = new Producer[num_producers];
		Consumer[] consumers = new Consumer[num_consumers];
		
		System.out.println("Inicializando ("+num_producers+") Produtores!");
		for (i=0; i < num_producers; i++){
		    producers[i] = new Producer(i + 1, (i+1) * 500, buffer);
		    producers[i].start();
		}
		
		System.out.println("Inicializando ("+num_consumers+") Consumidores!");
		for (i=0; i < num_consumers; i++){
		    consumers[i] = new Consumer(i + 1, (i+1) * 500, buffer);
		    consumers[i].start();
		}
	}
}