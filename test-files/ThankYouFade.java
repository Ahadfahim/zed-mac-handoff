import javax.swing.*;
import java.awt.*;

/**
 * ThankYouFade: "Thank you" shrinks from a big font to a tiny one, fading as it
 * shrinks, and then disappears.
 *
 * The animation is driven by a RECURSIVE method, shrink(size):
 *   - base case:       size < MIN_SIZE  -> hide the text and stop
 *   - recursive case:  draw the text at 'size', wait a moment, then call
 *                      shrink(size - STEP)
 *
 * The "wait a moment" uses a one-shot javax.swing.Timer instead of
 * Thread.sleep so the window stays responsive; the recursive call is made
 * from inside the timer's action, so each call still produces the next one.
 */
public class ThankYouFade extends JPanel {

    private static final String MESSAGE = "Thank you";
    private static final int START_SIZE = 96;   // biggest font
    private static final int MIN_SIZE   = 8;    // below this, the text disappears
    private static final int STEP       = 4;    // how much the font shrinks each call
    private static final int DELAY_MS   = 60;   // pause between calls

    private int fontSize = START_SIZE;
    private float alpha  = 1.0f;                // 1.0 = fully visible, 0.0 = invisible
    private boolean visible = true;

    public ThankYouFade() {
        setBackground(Color.WHITE);
        setPreferredSize(new Dimension(600, 250));
    }

    /** Recursive animation: each call draws one frame, then schedules the next call. */
    private void shrink(int size) {
        // Base case: too small to see, so make the text disappear and stop.
        if (size < MIN_SIZE) {
            visible = false;
            repaint();
            return;
        }

        // Do this call's work: smaller font, and lower opacity as we shrink.
        fontSize = size;
        alpha = (float) (size - MIN_SIZE) / (START_SIZE - MIN_SIZE);
        repaint();

        // Recursive case: after a short pause, shrink a little more.
        Timer timer = new Timer(DELAY_MS, e -> shrink(size - STEP));
        timer.setRepeats(false);
        timer.start();
    }

    /** Start (or restart) the animation. */
    public void start() {
        visible = true;
        shrink(START_SIZE);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (!visible) {
            return;                              // nothing to draw: it has disappeared
        }
        Graphics2D g2 = (Graphics2D) g.create();
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g2.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, Math.max(0f, Math.min(1f, alpha))));
        g2.setColor(new Color(30, 60, 120));
        g2.setFont(new Font("SansSerif", Font.BOLD, fontSize));

        FontMetrics fm = g2.getFontMetrics();
        int x = (getWidth() - fm.stringWidth(MESSAGE)) / 2;
        int y = (getHeight() - fm.getHeight()) / 2 + fm.getAscent();
        g2.drawString(MESSAGE, x, y);
        g2.dispose();
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            ThankYouFade panel = new ThankYouFade();

            JButton again = new JButton("Replay");
            again.addActionListener(e -> panel.start());

            JFrame frame = new JFrame("Thank you");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.add(panel, BorderLayout.CENTER);
            frame.add(again, BorderLayout.SOUTH);
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);

            panel.start();
        });
    }
}
