// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
===========================문제 01번============================
import java.util.Scanner;


public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int minutes = sc.nextInt();
        // 정수 나눗셈과 나머지를 사용합니다.
        // Use integer division and remainder.
        System.out.println("hours: " + minutes / 60);
        System.out.println("minutes: " + minutes % 60);
        sc.close();
    }
}
==================================================================
== 2번 문제 ==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;


public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int count = 0, sum = 0;
        // 짝수만 세고 더합니다.
        // Count and add only even numbers.
        for (int i = 1; i <= n; i++) {
            if (i % 2 == 0) { count++; sum += i; }
        }
        System.out.println("count: " + count);
        System.out.println("sum: " + sum);
        sc.close();
    }
}
=====================================================================
==3번 문제==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

class NameCard {
    private String name;
    // 생성자로 객체의 필드를 초기화합니다.
    // Initialize the object field with the constructor.
    public NameCard(String name) { this.name = name; }
    public String getName() { return name; }
    public int getLength() { return name.length(); }
    public char getInitial() { return name.charAt(0); }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        NameCard card = new NameCard(sc.next());
        System.out.println("name: " + card.getName());
        System.out.println("length: " + card.getLength());
        System.out.println("initial: " + card.getInitial());
        sc.close();
    }
}
===================================================================
==4번 문제==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

class Rectangle {
    private int width, height;
    // 다른 생성자에 초기화를 맡깁니다.
    // Delegate initialization to the other constructor.
    public Rectangle() { this(1, 1); }
    public Rectangle(int width, int height) { this.width = width; this.height = height; }
    public int getArea() { return width * height; }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int w = sc.nextInt(), h = sc.nextInt();
        Rectangle a = new Rectangle();
        Rectangle b = new Rectangle(w, h);
        System.out.println("default: " + a.getArea());
        System.out.println("custom: " + b.getArea());
        sc.close();
    }
}
========================================================================
== 5번 문제 ==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

class Wallet {
    private int balance;
    public Wallet(int balance) { this.balance = balance; }
    public int getBalance() { return balance; }
    public void deposit(int amount) { balance += amount; }
    public boolean withdraw(int amount) {
        // 실패하면 잔액을 변경하지 않습니다.
        // Leave the balance unchanged on failure.
        if (balance < amount) return false;
        balance -= amount;
        return true;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Wallet wallet = new Wallet(sc.nextInt());
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            String command = sc.next();
            int amount = sc.nextInt();
            boolean ok;
            if (command.equals("deposit")) { wallet.deposit(amount); ok = true; }
            else { ok = wallet.withdraw(amount); }
            System.out.println((ok ? "OK " : "FAIL ") + wallet.getBalance());
        }
        sc.close();
    }
}
===================================================================
== 6번 문제 ==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

class ScoreBox {
    private Integer value;
    // int를 Integer로 자동 박싱합니다.
    // Autobox int into Integer.
    public ScoreBox(int value) { this.value = value; }
    public int getValue() { return value; }
    // 계산에서 언박싱하고 반환할 때 박싱합니다.
    // Unbox for addition and box the returned result.
    public Integer addBonus(int bonus) { return value + bonus; }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String text = sc.next();
        int bonus = sc.nextInt();
        try {
            ScoreBox box = new ScoreBox(Integer.parseInt(text));
            System.out.println("score: " + box.getValue());
            System.out.println("bonus: " + box.addBonus(bonus));
        } catch (NumberFormatException e) {
            System.out.println("invalid");
        } finally {
            System.out.println("done");
        }
        sc.close();
    }
}
=================================================
==7번 문제==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

class Sim {
    protected String name;
    public Sim(String name) { this.name = name; }
    public void act() { System.out.println(name + " rests"); }
}
class YoungSim extends Sim {
    public YoungSim(String name) { super(name); }
    // 실제 객체가 YoungSim이면 이 메소드가 실행됩니다.
    // This override runs when the actual object is a YoungSim.
    @Override
    public void act() { System.out.println(name + " studies"); }
}
class Reporter {
    // 인수의 선언 타입에 따라 오버로드가 선택됩니다.
    // The argument's declared type selects the overload.
    public void report(Sim s) { System.out.println("type: Sim"); }
    public void report(YoungSim s) { System.out.println("type: YoungSim"); }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        YoungSim young = new YoungSim(sc.next());
        Sim sim = young;
        Reporter reporter = new Reporter();
        reporter.report(sim);
        sim.act();
        reporter.report(young);
        young.act();
        sc.close();
    }
}
==========================================================
==8번 문제==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

abstract class Sim {
    protected String name;
    public Sim(String name) { this.name = name; }
    public void rest() { System.out.println(name + " rests"); }
    // 자식 클래스에서 구현할 추상 메소드입니다.
    // Subclasses must implement this abstract method.
    public abstract void act();
}
class ChildSim extends Sim {
    public ChildSim(String name) { super(name); }
    @Override
    public void act() { super.rest(); System.out.println(name + " plays"); }
    public void toy() { System.out.println("toy: blocks"); }
}
class YoungSim extends Sim {
    public YoungSim(String name) { super(name); }
    @Override
    public void act() { super.rest(); System.out.println(name + " studies"); }
    public void subject() { System.out.println("subject: Java"); }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String name = sc.next();
        int age = sc.nextInt();
        Sim sim;
        if (age <= 12) sim = new ChildSim(name);
        else sim = new YoungSim(name);
        sim.act();
        // 실제 타입을 확인한 후 다운캐스팅합니다.
        // Downcast only after checking the actual type.
        if (sim instanceof ChildSim) {
            ChildSim child = (ChildSim) sim;
            child.toy();
        } else if (sim instanceof YoungSim) {
            YoungSim young = (YoungSim) sim;
            young.subject();
        }
        sc.close();
    }
}
==================================================
==9번 문제 ==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

interface Chargeable {
    // 인터페이스 상수는 모든 구현 객체가 공유합니다.
    // The interface constant is shared by all implementations.
    int MAX = 100;
    void charge(int amount);
    int getLevel();
}
class Battery implements Chargeable {
    private int level;
    public Battery(int level) { this.level = level; }
    @Override
    public void charge(int amount) {
        level += amount;
        if (level > Chargeable.MAX) level = Chargeable.MAX;
    }
    @Override
    public int getLevel() { return level; }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int initial = sc.nextInt(), a = sc.nextInt(), b = sc.nextInt();
        Chargeable battery = new Battery(initial);
        battery.charge(a);
        System.out.println("level: " + battery.getLevel());
        battery.charge(b);
        System.out.println("level: " + battery.getLevel());
        sc.close();
    }
}
===============================================================
==10번 문제==
// 홍길동(1111111)의 10점 예시 답안입니다.
// Sample 10-point answer for Hong Gil-dong (1111111).
import java.util.Scanner;

interface Named {
    String getName();
    int getId();
}
class Member implements Named {
    private String name;
    private int id;
    // 모든 Member 객체가 count 하나를 공유합니다.
    // All Member objects share one count field.
    private static int count = 0;
    public Member(String name) { this.name = name; count++; this.id = count; }
    @Override
    public String getName() { return name; }
    @Override
    public int getId() { return id; }
    public static int getCount() { return count; }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        Named[] members = new Named[n];
        for (int i = 0; i < n; i++) members[i] = new Member(sc.next());
        for (int i = 0; i < n; i++) {
            System.out.println(members[i].getId() + " " + members[i].getName());
        }
        System.out.println("count: " + Member.getCount());
        sc.close();
    }
}
==============================================================
