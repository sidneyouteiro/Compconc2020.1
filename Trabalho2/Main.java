import java.util.Queue;
import java.util.LinkedList;
import java.util.Random;
import java.util.Arrays; 

//classe usanda para delimitar os subarrays gerados pelos pivos
class WorkRange {
    public int start;
    public int finish;
}

//class usada para contar a quantidade de pivos que existem
class PivotCounter {
    private int counter;
    private int num_pivot;
    
    PivotCounter(int num_pivot) {
        this.counter = 0;
        this.num_pivot = num_pivot;
    }
    
    public synchronized void add() {
        this.counter++;
    }
    
    public synchronized boolean done() {
        if(this.counter >= this.num_pivot) {
            return true;
        }
        else {
            return false;
        }
    }
}

class Buffer {
    private Queue<WorkRange> buffer = new LinkedList<>();
    private int max_size;
    private PivotCounter pivot_counter;
    
    
    Buffer(PivotCounter pivot_counter) {
        this.pivot_counter = pivot_counter;
    }
    
    public synchronized void put(WorkRange value, int id) {
        buffer.add(value);
        System.out.println("Worker("+id+") adicionando elemento");
        notifyAll();
    }
    
    public synchronized WorkRange get(int id) {
        WorkRange error_range = new WorkRange();
        error_range.start = 0;
		error_range.finish = 0;
        try {
            while(buffer.size() == 0){
                if(this.pivot_counter.done()) {
                    System.out.println(this.pivot_counter.done());
                    return error_range;
                }
                System.out.println("Worker("+id+") bloqueado");
                wait();
            }
            WorkRange value = buffer.remove();
            System.out.println("Worker("+id+") retirando elemento");
            return value;
        } catch (InterruptedException e) { return error_range;}
    }
    
    public synchronized boolean has_elements() {
        if (buffer.size() == 0) {
            return false;
        } else {
            return true;
        }
    }
    
    public synchronized void print_buffer() {
        while(this.buffer.size() > 0){
            WorkRange value = buffer.remove();
            System.out.println(value.start);
            System.out.println(value.finish);
        }
    }
}

class Worker extends Thread {
    int id;
    Buffer buffer;
    int[] array;
    PivotCounter pivot_counter;
    
    Worker(int id, Buffer buffer, int[] array, PivotCounter pivot_counter) {
        this.id = id;
        this.buffer = buffer;
        this.array = array;
        this.pivot_counter = pivot_counter;
    }
    
    public void run() {
        boolean sent_new_range = true;
        int pivot_index = 0;
        while(!this.pivot_counter.done()) {
            sent_new_range = false;
            WorkRange range = this.buffer.get(this.id);
            int pivot = this.array[range.start];
            this.pivot_counter.add();
            boolean used_pivot = false;
            Queue<Integer> less_then = new LinkedList<>();
            Queue<Integer> more_then = new LinkedList<>();
            for (int i = range.start + 1; i < range.finish; i++) {
                if (this.array[i] >= pivot) {
                    more_then.add(this.array[i]);
                } else {
                    less_then.add(this.array[i]);
                }
    	    }
    	    for (int i = range.start; i < range.finish; i++) {
    	        if (less_then.size() > 0) {
    	            this.array[i] = less_then.remove();
    	        } 
    	        else if (used_pivot == false) {
    	            this.array[i] = pivot;
    	            pivot_index = i;
    	            used_pivot = true;
    	        } else {
    	            this.array[i] = more_then.remove(); 
    	        }
    	    }
    	    WorkRange less_partition = new WorkRange();
    	    WorkRange more_partition = new WorkRange();
    	    less_partition.start = range.start;
    	    less_partition.finish = pivot_index;
    	    more_partition.start = pivot_index + 1;
    	    more_partition.finish = range.finish;
    	    
    	    if (less_partition.finish - less_partition.start > 1) {
    	        this.buffer.put(less_partition, this.id);
    	        sent_new_range = true;
    	    } else if (less_partition.finish - less_partition.start == 1) {
    	        this.pivot_counter.add();
    	    }
    	    
    	    if (more_partition.finish - more_partition.start > 1) {
    	        this.buffer.put(more_partition, this.id);
    	        sent_new_range = true;
    	    } else if (more_partition.finish - more_partition.start == 1) {
    	        this.pivot_counter.add();
    	    }
        }
    }
}

public class Main{
    //quantidade threads
    static final int num_workers = 2;
    //tamanho do array
    static final int array_size = 10000;
    
	public static void main(String[] args) {
	    System.out.println("Começando o programa!");
	    
	    Random rd = new Random();
	    PivotCounter pivot_counter = new PivotCounter(array_size);
		Buffer buffer = new Buffer(pivot_counter);
		WorkRange inicial_range = new WorkRange();
		
		//geramos o array de Inteiros com valores aleatorios e sem duplicatas
		int[] array = new int[array_size];
		for (int i = 0; i < array.length; i++) {
            array[i] = rd.nextInt();
		}
		inicial_range.start = 0;
		inicial_range.finish = array_size;

        //inseriamos a primeira sublista no buffer, a lista original
		buffer.put(inicial_range, 0);

		Worker[] workers = new Worker[num_workers];
		System.out.println("Inicializando ("+num_workers+") Workers!");
		
        int i;
        long start = System.nanoTime();

        //instanciamos cada thread
		for (i=0; i < num_workers; i++){
		    workers[i] = new Worker(i + 1, buffer, array, pivot_counter);
		    workers[i].start();
		}
		//esperamos o fim da execução de todas as threads
		for (i=0; i < num_workers; i++){
		    try {
		        workers[i].join();
		    } catch (InterruptedException e) {}
		}
        long finish = System.nanoTime();
        long duration = (finish-start);

		buffer.print_buffer();
	
	    System.out.println(Arrays.toString(array));

        System.out.println("Duração(nanosegundos) do programa concorrente "+duration);

	}
}