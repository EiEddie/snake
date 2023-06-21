=======
Storage
=======
-------
储存
-------

将 **蛇身** (*body*) 分为数个方向不变, 呈直线状的 **节** (*section*), 按从 **蛇尾** (*tail*) 到 **蛇头** (*head*) 的方向编号.
如下图, 其中蛇头位置用 `h` 标记:

.. raw:: html

  <style>
  table.fig {
    border-collapse: collapse;
  }

  table.fig td {
    height: 50px;
    width: 50px;
    padding: 0px;
    border: 1px solid #000;
    text-align: center;
  }

  table.fig td[class] {
    background-color: #999;
  }

  table.fig td.r {
    border-right: none;
  }

  table.fig td.l {
    border-left: none;
  }

  table.fig td.u {
    border-top: none;
  }

  table.fig td.d {
    border-bottom: none;
  }
  </style>

.. raw:: html

  <table class="fig"><tbody>
  <tr>
    <td></td><td></td><td></td><td></td><td></td><td></td><td></td>
    <td class="d"></td>
    <td class="r">4</td>
    <td class="l"><i>h</i></td>
  </tr>
  <tr>
    <td class="r">0</td>
    <td class="l r"></td>
    <td class="l r"></td>
    <td class="l r"></td>
    <td class="l"></td>
    <td></td><td></td>
    <td class="u d"></td>
    <td></td><td></td>
  </tr>
  <tr>
    <td></td><td></td><td></td><td></td>
    <td class="d">1</td>
    <td></td><td></td>
    <td class="u">3</td>
    <td></td><td></td>
  </tr>
  <tr>
    <td></td><td></td><td></td><td></td>
    <td class="u"></td>
    <td class="r">2</td>
    <td class="l r"></td>
    <td class="l"></td>
    <td></td><td></td>
  </tr>
  </tbody></table>

图中共分5节, 规定每节的第一个格为自尾部向头部的第一个格, 即图中编号位置.
规定某节的位置即为这一节第一个格的位置.
当蛇身改变方向时出现新的一节, 改变方向后的第一个格即为新节的第一个格.

储存时, 应保存此节第一个格的 **位置** (*pos*), 此节的 **方向** (*dir*), 以及此节的 **长度** (*len*).
方向取向右为0, 顺时针依次增大.
即:

.. list-table::

  * - 0 (0b00)
    - 右
  * - 1 (0b01)
    - 下
  * - 2 (0b10)
    - 左
  * - 3 (0b11)
    - 上

图中第(0)节的长度, 方向分别为 `len=5`, `dir=0`.

当蛇直行时, 起始一节 (后称 **初节** (*first section*), 即第(0)节) 长度减1, 位置也随之变化;
最后一节 (后称 **末节** (*last section*), 即图中第(4)节) 长度加1, 但位置应保持不变.

当蛇因进食而蛇身长度增长时, 初节长度在移动时不变以使蛇身变长一格.

当蛇转向时, 应新建一节并将其作为末节.
